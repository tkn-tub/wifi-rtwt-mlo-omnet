//
//  Created on: April 2025
//      Author: Mateusz Zakrzewski, Doganalp Ergenc (TU Berlin)
//

// U-MAC module for R-TWT enabled STA (generic).
// - It extends the base U-MAC implementation from the MLO project (Umac).
// - STAs announce their link requirements provided in the .ini file.
// - SLO mode is also available.

#include "inet/linklayer/common/InterfaceTag_m.h"
#include "inet/linklayer/common/UserPriority.h"
#include "inet/linklayer/common/UserPriorityTag_m.h"

//added by zakrzewski
#include "RtwtUMac.h"
#include "inet/common/packet/Packet.h"
#include "inet/linklayer/ethernet/common/EthernetMacHeader_m.h"
#include "inet/linklayer/common/MacAddressTag_m.h"
#include "inet/common/ProtocolTag_m.h"
#include "inet/common/ModuleAccess.h"
#include "inet/linklayer/ieee80211/mgmt/Ieee80211MgmtSta.h"
#include "inet/linklayer/ieee80211/mgmt/Ieee80211MgmtAp.h"
#include "inet/linklayer/ethernet/common/EthernetMacHeader_m.h"
#include "inet/linklayer/ieee80211/mgmt/Ieee80211MgmtFrame_m.h"
#include "inet/networklayer/common/InterfaceTable.h"
#include "inet/linklayer/common/Ieee802SapTag_m.h"
#include "inet/common/packet/chunk/BytesChunk.h"

#include "inet/common/packet/Message_m.h"
#include "src/rtwt/msg/twtFrame_m.h"
#include "src/rtwt/msg/twtLinkRequirement_m.h"
#include "inet/linklayer/ieee80211/mac/Ieee80211SubtypeTag_m.h"

#include "../lmac/mac/Ieee80211beRtwtProtocol.h"
#include "../rtwt/TwtTimeConverter.h"


namespace inet {

using namespace std;

Register_Class(RtwtUMac);

void RtwtUMac::initialize(int stage)
{
    UMac::initialize(stage);
    if (stage == INITSTAGE_LOCAL) {
        // MODIFICATION BEGIN: Reading R-TWT parameters
        sloLink = par("sloLink");
        if(isStation && hasPar("twtEnable") && par("twtEnable")){
            if (hasPar("linkRequirements")) {
                scheduleAt(uniform(SimTime(2.0, SIMTIME_S), SimTime(2.5, SIMTIME_S)), new cMessage("TWTNegotiateEvent")); // Start TWT Negotiation between 0.75 and 0.85s SimTime
            }
        }
        // MODIFICATION END
    }
}

void RtwtUMac::handleUpperPacket(Packet *packet) {

    // MODIFICATION BEGIN: R-TWT extension
    // setting link layer entry timestamp on packet for statistics
    if(isStation){
        packet->addPar("sendTime").setDoubleValue(simTime().dbl());
    }
    cModule* host = getContainingNode(this);
    std::string hostName = host->getName();
    std::string statName = hostName + "_umac_" + packet->getName() + "_entry";
    recordScalar(statName.c_str(), simTime().dbl());

    packet->removeTagIfPresent<DispatchProtocolReq>();
    MacAddress destAddr;
    if(isStation){
        destAddr = macAddr; //STA has only one queue with its MAC Address
    } else {
        destAddr = packet->getTag<MacAddressReq>()->getDestAddress();   // AP Extracts the destination MAC from the packet
    }
    // implements multiple queues that holds the packets for different destinations (by MAC) - for STA, it means only one queue.
    pendingByDest[destAddr].push_back(packet); // Buffer centrally packets, notify about new packets

    for(int i = 1; i >= 0; i--){ //Start with 5GHz link
        Packet* notify = new Packet("newPackets");
        NetworkInterface *targetIft = ift->getInterface(i);
        notify->addTagIfAbsent<InterfaceReq>()->setInterfaceId(targetIft->getInterfaceId());
        send(notify, lowerLayerOutGateId);
    }
    // MODIFICATION END
}

void RtwtUMac::handleLowerPacket(Packet *packet) {
    //std::cout << this->getParentModule()->getName() << "." << this->getName() << "." <<  __func__ << "() logs - Received " << packet->getName() << " from " << packet->getSenderModule()->getParentModule()->getFullName() << endl;

    updateLinkQosParameters(packet);

    // TODO: Here first check if management or control frames are forwarded here.

    // replace 802.11be indicators with upper layer indicator
    // identified by LLC layer
    auto upperProtocol = packet->getTag<MacProtocolInd>()->getProtocol();

    packet->addTagIfAbsent<DispatchProtocolReq>()->setProtocol(upperProtocol);
    packet->addTagIfAbsent<PacketProtocolTag>()->setProtocol(upperProtocol);

    if(!isStation) {
        // IMPORTANT: AP keeps tracks of the received interface of each frame.
        packet->addTagIfAbsent<InterfaceReq>()->setInterfaceId(packet->getTag<InterfaceInd>()->getInterfaceId());
    } else {
        updateStatistics(packet);
        string packetName = packet->getName();
        if(packetName.find("UDPLinkQuality") != string::npos) {
            delete packet;
            return;
        }
    }

    // MODIFICATION BEGIN: For R-TWT statistics collection
    if (packet->hasPar("sendTime")) {
        // Calculate latency using entry timestamp
        simtime_t latency = simTime() - SimTime(packet->par("sendTime").doubleValue());
        cModule* host = getContainingNode(this);
        std::string hostName = host->getName();
        std::string exitStatName = hostName + "_umac_" + packet->getName() + "_exit";
        recordScalar(exitStatName.c_str(), simTime().dbl());
        std::string latencyStatName = hostName + "_umac_" + packet->getName() + "_latency";
        recordScalar(latencyStatName.c_str(), latency.dbl());
    }
    // MODIFICATION END
    send(packet, upperLayerOutGateId);
}

void RtwtUMac::handleSelfMessage(cMessage *message) {

    // MODIFICATION BEGIN: Handle trigger for R-TWT negotiation and wake/sleep timers
    if (strcmp(message->getName(), "TWTNegotiateEvent") == 0) {
        parseLinkRequirements();
        delete message;
        return;
    }
    // MODIFICATION END
    UMac::handleSelfMessage(message);
}

void RtwtUMac::parseLinkRequirements(){
    cValueArray *twtParamsArray = check_and_cast<cValueArray *>(par("linkRequirements").objectValue());
    for (int i = 0; i < twtParamsArray->size(); i++) {
        cValueMap *twtParams = check_and_cast<cValueMap *>(twtParamsArray->get(i).objectValue());

        int flowId = twtParams->get("flowId").intValue();
        int restriction = twtParams->get("restriction").intValue();
        int dataRate = twtParams->get("dataRate").intValue();
        MacAddress receiver = MacAddress(twtParams->get("receiver").stringValue());
        uint32_t latency = twtParams->get("latency").intValue();
        int wakeTime = twtParams->get("wakeTime").intValue();
        EV_INFO << "Parsed TWT link requirements: flowId=" << flowId
                << ", restriction=" << restriction
                << ", dataRate=" << dataRate
                << ", latency=" << latency
                << ", receiver=" << receiver
                << endl;

        sendTwtLinkRequirements("twt-requirements", restriction, dataRate, receiver, latency, macAddr, wakeTime);
    }
}

void RtwtUMac::sendTwtLinkRequirements(std::string name, int restriction, int dataRate, MacAddress receiver, uint32_t latency, MacAddress dest, int wakeTime){
    Packet *pkt = new Packet(name.c_str());

    // Create a TWT frame with the parameters
    const auto& body = makeShared<twtLinkRequirement>();
    body->setRestriction(restriction);
    body->setReceiver(receiver);
    body->setDataRate(dataRate);
    body->setWakeTime(wakeTime);
    body->setLatency(latency);
    body->setSTAaddress(dest);
    body->setChunkLength(B(25));

    pkt->addTag<PacketProtocolTag>()->setProtocol(ieee80211beRtwt);
    pkt->insertAtBack(body);
    pkt->setKind(1); //Request
    NetworkInterface *targetIft = ift->getInterface(0); //Send requests always over 2,4GHz (link 0)
    if(targetIft->getState() == NetworkInterface::State::UP) {
       pkt->addTagIfAbsent<InterfaceReq>()->setInterfaceId(targetIft->getInterfaceId());
       send(pkt, lowerLayerOutGateId);
    }

}

void RtwtUMac::sendTwtPacket(std::string name, int flowId, int restriction, int interfaceId, int type, simtime_t wakeTime, int wakeDuration, int intervalMantissa, int intervalExponent, MacAddress dest) {
    Packet *pkt = new Packet(name.c_str());
    // Create a TWT frame with the parameters
    const auto& body = makeShared<twtFrame>();
    body->setFlowId(flowId);
    body->setType(type);
    body->setRestriction(restriction);
    body->setWakeTime(wakeTime);
    body->setWakeDuration(wakeDuration);
    body->setIntervalMantissa(intervalMantissa);
    body->setIntervalExponent(intervalExponent);
    body->setSTAaddress(dest);
    body->setChunkLength(B(22));
    body->setInterfaceId(interfaceId);
    pkt->addTag<PacketProtocolTag>()->setProtocol(ieee80211beRtwt);
    pkt->setKind(0); //Response
    pkt->insertAtBack(body);
    if(interfaceId>=100){
        interfaceId -=100;
    }
    NetworkInterface *targetIft = ift->getInterface(interfaceId);
    if(targetIft->getState() == NetworkInterface::State::UP) {
        pkt->addTagIfAbsent<InterfaceReq>()->setInterfaceId(targetIft->getInterfaceId());
        send(pkt, lowerLayerOutGateId);
    }
}

void RtwtUMac::pullPacket(MacAddress awakeMac, int activeSp, int iface) { // Dequeue packet from the right queue
    if(sloLink != -1){
        if(sloLink != iface){ // use single link, do not push packets to the link
            return;
        }
    }
    if(!par("twtEnable")){  //Standard operation without TWT
        bool packetPulled = false;
        for (auto it = pendingByDest.begin(); it != pendingByDest.end() && !packetPulled; ++it) {
            if (!it->second.empty()) {
                Packet *pkt = it->second.front();
                it->second.pop_front();
                NetworkInterface *targetIft = ift->getInterface(iface);
                pkt->addTagIfAbsent<InterfaceReq>()->setInterfaceId(targetIft->getInterfaceId());
                send(pkt, lowerLayerOutGateId);
                packetPulled = true;
                break;
            }
        }
        if(!packetPulled) {
            EV_DEBUG << "No packets available to enqueue during active TWT SP" << endl;;
        }
        return;
    }

    if(awakeMac != MacAddress::UNSPECIFIED_ADDRESS && !par("isStation")){   // AP's handler for an active TWT SP
        auto it = pendingByDest.find(awakeMac);
        if (it != pendingByDest.end()) {
            auto &dq = it->second;
            if (!dq.empty()) {
                Packet *pkt = dq.front();
                dq.pop_front();
                EV_INFO << "flushing one pkt to " << awakeMac << " (remaining: " << dq.size() << ")" << endl;
                NetworkInterface *targetIft = ift->getInterface(iface);
                pkt->addTagIfAbsent<InterfaceReq>()->setInterfaceId(targetIft->getInterfaceId());
                send(pkt, lowerLayerOutGateId);
            } 
        } 
    } else if (activeSp == 2){  //Individual TWT SP handler for STA
        bool packetPulled = false;
        for (auto it = pendingByDest.begin(); it != pendingByDest.end() && !packetPulled; ++it) {
            if (!it->second.empty()) {
                Packet *pkt = it->second.front();
                it->second.pop_front();
                NetworkInterface *targetIft = ift->getInterface(iface);
                pkt->addTagIfAbsent<InterfaceReq>()->setInterfaceId(targetIft->getInterfaceId());
                send(pkt, lowerLayerOutGateId);
                packetPulled = true;
                break;
            }
        }
        if(!packetPulled) {
            EV_DEBUG << "No packets available to enqueue during active TWT SP\n";
        }
    } else {
        EV_DEBUG << "No active TWT SP detected during frame sequence finish\n";
    }
}

}
