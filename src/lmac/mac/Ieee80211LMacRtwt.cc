//
//  Created on: April 2025
//      Author: Mateusz Zakrzewski (TU Berlin)
//
// This module extends the L-MAC interfaces in the base MLO framework (Ieee80211LMac).
// It includes a new R-TWT agent, and uses the extended R-TWT-aware HCF.
//

#include "inet/common/INETUtils.h"
#include "inet/common/ModuleAccess.h"
#include "inet/common/ProtocolTag_m.h"
#include "inet/common/packet/Message.h"
#include "inet/common/packet/Packet.h"
#include "inet/linklayer/common/InterfaceTag_m.h"
#include "inet/linklayer/common/MacAddressTag_m.h"
#include "inet/linklayer/common/UserPriorityTag_m.h"
#include "inet/linklayer/ieee80211/llc/IIeee80211Llc.h"
#include "inet/linklayer/ieee80211/llc/LlcProtocolTag_m.h"
#include "inet/linklayer/ieee80211/mac/Ieee80211Frame_m.h"
#include "inet/linklayer/ieee80211/mac/Ieee80211SubtypeTag_m.h"
#include "inet/linklayer/ieee80211/mac/Rx.h"
#include "inet/linklayer/ieee80211/mac/contract/IContention.h"
#include "inet/linklayer/ieee80211/mac/contract/IFrameSequence.h"
#include "inet/linklayer/ieee80211/mac/contract/IRx.h"
#include "inet/linklayer/ieee80211/mac/contract/ITx.h"
#include "inet/networklayer/contract/IInterfaceTable.h"
#include "inet/physicallayer/wireless/ieee80211/packetlevel/Ieee80211ControlInfo_m.h"
#include "inet/physicallayer/wireless/ieee80211/packetlevel/Ieee80211Tag_m.h"
#include "inet/common/Protocol.h"
#include "inet/linklayer/common/InterfaceTag_m.h"
#include "inet/common/socket/SocketTag_m.h"
#include "inet/linklayer/ieee80211/mac/Ieee80211Frame_m.h"
#include "inet/common/IProtocolRegistrationListener.h"
#include "inet/common/Compat.h"

#include "src/rtwt/msg/twtScheduleBeacon_m.h"
#include "Ieee80211beRtwtProtocol.h"
#include "Ieee80211LMacRtwt.h"

namespace inet {
namespace ieee80211 {


using namespace inet::physicallayer;

Define_Module(Ieee80211LMacRtwt);

Ieee80211LMacRtwt::Ieee80211LMacRtwt()
{
}

Ieee80211LMacRtwt::~Ieee80211LMacRtwt()
{
    if (pendingRadioConfigMsg)
        delete pendingRadioConfigMsg;
}

void Ieee80211LMacRtwt::initialize(int stage)
{
    MacProtocolBase::initialize(stage);
    if (stage == INITSTAGE_LOCAL) {
        modeSet = Ieee80211ModeSet::getModeSet(par("modeSet"));
        fcsMode = parseFcsMode(par("fcsMode"));
        mib.reference(this, "mibModule", true);
        mib->qos = par("qosStation");
    }
    else if (stage == INITSTAGE_LINK_LAYER) {
        cModule *llcModule = gate("upperLayerOut")->getNextGate()->getOwnerModule();
        llc = check_and_cast<IIeee80211Llc *>(llcModule);
        cModule *radioModule = gate("lowerLayerOut")->getNextGate()->getOwnerModule();
        radioModule->subscribe(IRadio::radioModeChangedSignal, this);
        radioModule->subscribe(IRadio::receptionStateChangedSignal, this);
        radioModule->subscribe(IRadio::transmissionStateChangedSignal, this);
        radioModule->subscribe(IRadio::receivedSignalPartChangedSignal, this);
        radio = check_and_cast<IRadio *>(radioModule);
        ds = check_and_cast<IDs *>(getSubmodule("ds"));
        rx = check_and_cast<IRx *>(getSubmodule("rx"));
        tx = check_and_cast<ITx *>(getSubmodule("tx"));
        emit(modesetChangedSignal, modeSet);
        if (isUp())
            initializeRadioMode();
        rx = check_and_cast<IRx *>(getSubmodule("rx"));
        tx = check_and_cast<ITx *>(getSubmodule("tx"));
        dcf = check_and_cast<Dcf *>(getSubmodule("dcf"));
        // MODIFICATION BEGIN: Extended R-TWT HCF and TWT agents are added
        hcf = check_and_cast_nullable<LHcfRtwt *>(getSubmodule("hcf"));
        cModule *twtAgentModule = gate("twtOut")->getNextGate()->getOwnerModule();
        twtAgent = check_and_cast<inet::ieee80211::TwtAgent *>(twtAgentModule);
        registerService(*ieee80211beRtwt, gate("twtIn"), gate("twtOut"));
        // MODIFICATION END
        if (mib->qos && !hcf)
            throw cRuntimeError("Missing hcf module, required for QoS");
    } else if (stage == INITSTAGE_NETWORK_CONFIGURATION) {
        // IMPORTANT: MIB address is used for packet encapsulation - should be changed after UMAC reset MAC addresses in LINK_LAYER configuration stage.
        mib->address = networkInterface->getMacAddress();
    }
}

std::vector<ChannelAccessParameters> Ieee80211LMacRtwt::getChannelAccessParams() {

    std::vector<ChannelAccessParameters> params = hcf->getChannelAccessParams();

    int ifaceId = networkInterface->getInterfaceId();

    for (int i = 0; i < params.size(); i++) {
        params[i].ifaceId = ifaceId;
    }
    return params;
}

void Ieee80211LMacRtwt::handleLowerPacket(Packet *packet)
{
    if (rx->lowerFrameReceived(packet)) {
        auto header = packet->peekAtFront<Ieee80211MacHeader>();
        processLowerFrame(packet, header);
    }
    else { // corrupted frame received
        if (mib->qos)
            hcf->corruptedFrameReceived();
        else
            dcf->corruptedFrameReceived();
    }
}

void Ieee80211LMacRtwt::encapsulate(Packet *packet)
{
    packet->addTagIfAbsent<LlcProtocolTag>()->setProtocol(packet->getTag<PacketProtocolTag>()->getProtocol());
    auto macAddressReq = packet->getTag<MacAddressReq>();
    auto destAddress = macAddressReq->getDestAddress();
    const auto& header = makeShared<Ieee80211DataHeader>();
    header->setTransmitterAddress(mib->address);
    if (mib->mode == Ieee80211Mib::INDEPENDENT)
        header->setReceiverAddress(destAddress);
    else if (mib->mode == Ieee80211Mib::INFRASTRUCTURE) {
        if (mib->bssStationData.stationType == Ieee80211Mib::ACCESS_POINT) {
            header->setFromDS(true);
            // IMPORTANT: Address 3 of a frame from the access point (FromDS) is the source address (SA), as in 802.11,
            // so that the receiver knows the station the frame comes from. INET's Ieee80211Mac::encapsulate() puts the
            // address of the access point there instead. The access point itself is the source of its own frames.
            auto srcAddress = macAddressReq->getSrcAddress();
            header->setAddress3(srcAddress.isUnspecified() ? mib->address : srcAddress);
            header->setReceiverAddress(destAddress);
        }
        else if (mib->bssStationData.stationType == Ieee80211Mib::STATION) {
            header->setToDS(true);
            header->setReceiverAddress(mib->bssData.bssid);
            header->setAddress3(destAddress);
        }
        else
            throw cRuntimeError("Unknown station type");
    }
    else
        throw cRuntimeError("Unknown mode");
    if (auto userPriorityReq = packet->findTag<UserPriorityReq>()) {
        // make it a QoS frame, and set TID
        header->setType(ST_DATA_WITH_QOS);
        header->addChunkLength(QOSCONTROL_PART_LENGTH);
        header->setTid(userPriorityReq->getUserPriority());
    }
    packet->insertAtFront(header);
    packet->insertAtBack(makeShared<Ieee80211MacTrailer>());
    auto packetProtocolTag = packet->addTagIfAbsent<PacketProtocolTag>();
    packetProtocolTag->setProtocol(&Protocol::ieee80211Mac);
}

void Ieee80211LMacRtwt::decapsulate(Packet *packet)
{
    const auto& header = packet->popAtFront<Ieee80211DataOrMgmtHeader>();
    auto packetProtocolTag = packet->addTagIfAbsent<PacketProtocolTag>();
    if (dynamicPtrCast<const Ieee80211DataHeader>(header))
        packetProtocolTag->setProtocol(llc->getProtocol());
    else if (dynamicPtrCast<const Ieee80211MgmtHeader>(header))
        packetProtocolTag->setProtocol(&Protocol::ieee80211Mgmt);
    auto macAddressInd = packet->addTagIfAbsent<MacAddressInd>();
    if (mib->mode == Ieee80211Mib::INDEPENDENT) {
        macAddressInd->setSrcAddress(header->getTransmitterAddress());
        macAddressInd->setDestAddress(header->getReceiverAddress());
    }
    else if (mib->mode == Ieee80211Mib::INFRASTRUCTURE) {
        if (mib->bssStationData.stationType == Ieee80211Mib::ACCESS_POINT) {
            macAddressInd->setSrcAddress(header->getTransmitterAddress());
            macAddressInd->setDestAddress(header->getAddress3());
        }
        else if (mib->bssStationData.stationType == Ieee80211Mib::STATION) {
            macAddressInd->setSrcAddress(header->getAddress3());
            macAddressInd->setDestAddress(header->getReceiverAddress());
        }
        else
            throw cRuntimeError("Unknown station type");
    }
    else
        throw cRuntimeError("Unknown mode");
    if (header->getType() == ST_DATA_WITH_QOS) {
        auto dataHeader = dynamicPtrCast<const Ieee80211DataHeader>(header);
        int tid = dataHeader->getTid();
        if (tid < 8)
            packet->addTagIfAbsent<UserPriorityInd>()->setUserPriority(tid);
    }
    packet->addTagIfAbsent<InterfaceInd>()->setInterfaceId(networkInterface->getInterfaceId());
    packet->popAtBack<Ieee80211MacTrailer>(B(4));
}

void Ieee80211LMacRtwt::handleMessageWhenUp(cMessage *message)
{
    if(strcmp(message->getName(), "newPackets") == 0){
        hcf->getNewPackets();
        delete message;
        return;
    }
    if (message->arrivedOn("mgmtIn")) {
        if (!message->isPacket())
            handleUpperCommand(message);
        else
            handleMgmtPacket(check_and_cast<Packet *>(message));
    } else if (message->arrivedOn("twtIn")){
        handleTwtPacket(check_and_cast<Packet *>(message)); //Packets to forward to other nodes on the network.
    }
    else
        LayeredProtocolBase::handleMessageWhenUp(message);
}

void Ieee80211LMacRtwt::handleTwtWakeUp(){
    hcf->handleTwtWakeUp();
}

void Ieee80211LMacRtwt::handleTwtPacket(Packet *packet)
{
    // Handle TWT wake/sleep state changes
    if(strcmp(packet->getName(), "twtSleep") == 0) {
        EV_DEBUG << "Sleep handling is not implemented in L-MAC"<<endl;
        delete packet;
        return;
    }
    else if(strcmp(packet->getName(), "twtWakeUp") == 0) {
        EV_INFO << "TWT Wake signal received at LMac at " << simTime() << endl;
        handleTwtWakeUp();
        delete packet;
        return;
    }
    
    //Forwarding TWT Packet to other Hosts
    const auto& chunk = packet->peekData();
    const auto& header = makeShared<Ieee80211MgmtHeader>();
    header->setType((Ieee80211FrameType)packet->getTag<Ieee80211SubtypeReq>()->getSubtype());
    if(strcmp(packet->getName(), "TWT_Beacon") == 0){ //Forward requests to other node (AP)
        if(auto beaconChunk = dynamicPtrCast<const twtScheduleBeacon>(chunk)){
            packet->addTagIfAbsent<Ieee80211SubtypeReq>()->setSubtype(ST_BEACON);
            header->setReceiverAddress(MacAddress::BROADCAST_ADDRESS);
        }
    } else {
        auto twtChunk = dynamicPtrCast<const twtFrame>(chunk);
        if (mib->mode == Ieee80211Mib::INFRASTRUCTURE && mib->bssStationData.stationType == Ieee80211Mib::ACCESS_POINT){
            //Response from AP
            if(twtChunk->getType()==0 && !twtChunk->getSTAaddress().isUnspecified()){
                header->setReceiverAddress(twtChunk->getSTAaddress());
            } else { //Request from AP. AP does not generate requests.
                return;
            }
        } else if (mib->mode == Ieee80211Mib::INFRASTRUCTURE && mib->bssStationData.stationType == Ieee80211Mib::STATION){
            //Request and response from STA. Send always to AP
            header->setReceiverAddress(mib->bssData.bssid);
        }
    }
    //Add receiver MAC address based on AP / STA and Request Type.
    EV << header->getReceiverAddress() << " " << mib->bssData.bssid << endl;
    packet->insertAtFront(header);
    packet->insertAtBack(makeShared<Ieee80211MacTrailer>());
    processUpperFrame(packet, header);
}

void Ieee80211LMacRtwt::sendUp(cMessage *msg)
{
    Enter_Method("sendUp(\"%s\")", msg->getName());
    take(msg);

    Packet* frame = check_and_cast<Packet *>(msg);

    // Update protocol to be sent to UMAC
    frame->addTagIfAbsent<DispatchProtocolReq>()->setProtocol(ieee80211be);
    frame->addTagIfAbsent<PacketProtocolTag>()->setProtocol(ieee80211be);
    frame->addTagIfAbsent<InterfaceInd>()->setInterfaceId(networkInterface->getInterfaceId());

    MacProtocolBase::sendUp(frame);
}

void Ieee80211LMacRtwt::sendUpFrame(Packet *frame)
{
    Enter_Method("sendUpFrame(\"%s\")", frame->getName());
    take(frame);
    const auto& header = frame->peekAtFront<Ieee80211DataOrMgmtHeader>();
    decapsulate(frame);

    // Update protocol to be sent to UMAC
    frame->addTagIfAbsent<DispatchProtocolReq>()->setProtocol(ieee80211be);
    frame->addTagIfAbsent<PacketProtocolTag>()->setProtocol(ieee80211be);
    frame->addTagIfAbsent<InterfaceInd>()->setInterfaceId(networkInterface->getInterfaceId());

    // MODIFICATION START
    // Statistics
    cModule* host = getContainingNode(this);
    std::string hostName = host->getName();
    std::string interfaceId = std::to_string(getParentModule()->getIndex());
    std::string exitStatName = hostName + "_lmac[" + interfaceId + "]_" + frame->getName() + "_exit";
    recordScalar(exitStatName.c_str(), simTime().dbl());

    // Sending TWT Message to U-MAC
    EV << "Packet of len " << frame->getDataLength() << endl;
    const auto& chunk = frame->peekData();
    if(strcmp(frame->getName(), "twt-requirements") == 0){
        send(frame, "twtOut");
        return;
    } else if(strcmp(frame->getName(), "TWT_Beacon") == 0){
        send(frame, "twtOut");
        return;
    }
    if (chunk->getChunkType() == Chunk::CT_FIELDS) {
        if (auto twtChunk = dynamicPtrCast<const twtFrame>(chunk)) {
            send(frame, "twtOut");
            return;
        }
    }
    // MODIFICATION END
    if (!(header->getType() & 0x30)) {
        send(frame, "mgmtOut");
    }
    else
        ds->processDataFrame(frame, dynamicPtrCast<const Ieee80211DataHeader>(header));

}

void Ieee80211LMacRtwt::processUpperFrame(Packet *packet, const Ptr<const Ieee80211DataOrMgmtHeader>& header)
{
    Enter_Method("processUpperFrame(\"%s\")", packet->getName());
    take(packet);
    EV_INFO << "Frame " << packet << " received from higher layer, receiver = " << header->getReceiverAddress() << "\n";
    ASSERT(!header->getReceiverAddress().isUnspecified());

    if (mib->qos)
        hcf->processUpperFrame(packet, header);
    else
        dcf->processUpperFrame(packet, header);
}

void Ieee80211LMacRtwt::processLowerFrame(Packet *packet, const Ptr<const Ieee80211MacHeader>& header)
{
    Enter_Method("processLowerFrame(\"%s\")", packet->getName());
    take(packet);

    if (mib->qos) {
        /* IMPORTANT:
         * Update here to send management and control frames to U-MAC
         * This potentially needs additional tagging.

        if (auto mgmtHeader = dynamicPtrCast<const Ieee80211MgmtHeader>(header)) {
            sendUp(packet->dup());
        }
        else (auto ackFrame = dynamicPtrCast<const Ieee80211AckFrame>(header)) {
            sendUp(packet->dup());
        }
        */
        hcf->processLowerFrame(packet, header);
    }
    else
        // TODO what if the received frame is ST_DATA_WITH_QOS? drop?
        dcf->processLowerFrame(packet, header);
}

} // namespace ieee80211
} // namespace inet

