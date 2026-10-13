/*
 * Ieee80211LMgmtAp.cc
 *
 *      Author: zakrzewski
 */
#include "inet/common/ModuleAccess.h"
#include "inet/linklayer/common/InterfaceTag_m.h"
#include "inet/linklayer/common/MacAddressTag_m.h"
#include "inet/linklayer/ieee80211/mac/Ieee80211Frame_m.h"
#include "inet/common/ProtocolTag_m.h"
#include "Ieee80211LMgmtApRtwt.h"

#include "src/lmac/mac/Ieee80211beProtocol.h"

#include "src/rtwt/msg/beaconTimers_m.h"
#include "src/rtwt/msg/managementInfoFrame_m.h"
#include "src/lmac/mac/Ieee80211beRtwtProtocol.h"

namespace inet {
namespace ieee80211 {

Define_Module(Ieee80211LMgmtApRtwt);

void Ieee80211LMgmtApRtwt::handleAssociationRequestFrame(Packet *packet, const Ptr<const Ieee80211MgmtHeader>& header) {
    Ieee80211MgmtAp::handleAssociationRequestFrame(packet, header);
    // send management info to U-MAC about association
    // Get MAC header instead of management header
    if(hasPar("twtEnable") && par("twtEnable")) {
        MacAddress staAddress = header->getTransmitterAddress();
        sendManagementInfo(staAddress, 1);
    }
}

void Ieee80211LMgmtApRtwt::sendManagementInfo(const MacAddress& staAddr, uint8_t status) {
    // Create management info frame about new association
    auto frame = makeShared<ManagementInfoFrame>();
    EV_INFO << "sendManagementInfo: " << getParentModule()->getIndex() << endl;
    frame->setStaAddress(staAddr);
    frame->setInterfaceId(getParentModule()->getIndex());
    frame->setStatus(status);
    frame->setChunkLength(B(8));
    Packet *packet = new Packet("MgmtInfo");
    auto req = packet->addTagIfAbsent<DispatchProtocolReq>();
    req->setProtocol(ieee80211be);
    req->setServicePrimitive(SP_INDICATION);
    packet->addTagIfAbsent<MacAddressInd>()->setSrcAddress(staAddr);
    packet->addTagIfAbsent<InterfaceInd>()->setInterfaceId(getParentModule()->getIndex());
    packet->addTagIfAbsent<MacProtocolInd>()->setProtocol(ieee80211be);
    packet->setKind(1);
    packet->insertAtBack(frame);
    send(packet, "llcOut");
}

void Ieee80211LMgmtApRtwt::sendBeaconTimers(simtime_t startTime, simtime_t beaconInterval) {
    // Inform U-MAC about beacon timer parameters
    auto frame = makeShared<beaconTimers>();
    EV_INFO << "Sending Beacon Timers " << getParentModule()->getIndex() << endl;
    frame->setStartTime(startTime);
    frame->setBeaconInterval(beaconInterval);
    frame->setChunkLength(B(16));
    Packet *packet = new Packet("beaconInfo");

    auto req = packet->addTagIfAbsent<DispatchProtocolReq>();
    req->setProtocol(ieee80211be);
    req->setServicePrimitive(SP_INDICATION);
    packet->addTagIfAbsent<InterfaceInd>()->setInterfaceId(getParentModule()->getIndex());
    packet->addTagIfAbsent<MacProtocolInd>()->setProtocol(ieee80211beRtwt);
    packet->insertAtBack(frame);
    send(packet, "llcOut");
}

void Ieee80211LMgmtApRtwt::start(){
    Ieee80211MgmtApBase::start();
    simtime_t start = 0;   //Set same beacon start time = 0 for both interfaces
    scheduleAfter(start, beaconTimer);
    if(hasPar("twtEnable") && par("twtEnable")){// Send beacon timers only if TWT is enabled
        sendBeaconTimers(start, beaconInterval);    
    }
}


}
}
