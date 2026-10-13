//
//  Created on: April 2025
//      Author: Mateusz Zakrzewski, Doganalp Ergenc (TU Berlin)
//

// U-MAC module for R-TWT enabled AP.
// - It extends the base R-TWT enabled U-MAC implementation (RtwtUmac).
// - It has a special queueing facility to orchestrate transmissions within the wake times of the destination STAs.
// - To be able to use R-TWT, it should deploy a scheduler. Symmetrical, asymmetrical, and cross-symmetrical schedulers are available.

#include <omnetpp.h>

#include "inet/linklayer/common/InterfaceTag_m.h"
#include "inet/networklayer/common/InterfaceTable.h"
#include "inet/common/ProtocolTag_m.h"

#include "src/rtwt/TwtTimeConverter.h"
#include "src/rtwt/msg/twtLinkRequirement_m.h"
#include "src/rtwt/msg/beaconTimers_m.h"
#include "src/rtwt/msg/twtScheduleBeacon_m.h"
#include "src/rtwt/msg/managementInfoFrame_m.h"

#include "src/rtwt/scheduler/TwtSchedulerBase.h"
#include "src/lmac/mac/Ieee80211beRtwtProtocol.h"

#include "RtwtUMacAccessPoint.h"

namespace inet {

Register_Class(RtwtUMacAccessPoint);

void RtwtUMacAccessPoint::initialize(int stage) {
    RtwtUMac::initialize(stage);
    if(stage == INITSTAGE_LINK_LAYER){
        scheduler = check_and_cast<TwtSchedulerBase *>(getSubmodule("twtScheduler"));
    }
}

void RtwtUMacAccessPoint::handleSelfMessage(cMessage *message){
    if(strncmp(message->getName(), "twt-", 4)==0){
        if(!isStation){
            handleApTwtTimer(message);
            return;
        }
    }
    RtwtUMac::handleSelfMessage(message);
}

void RtwtUMacAccessPoint::handleLowerPacket(Packet *packet) {
    auto upperProtocol = packet->getTag<MacProtocolInd>()->getProtocol();
    //handle new packet types for TWT and Management Info correctly
    if(upperProtocol == ieee80211beRtwt){
        handleTwtPacket(packet);
        return;
    }
    if (upperProtocol == ieee80211be && packet->getKind() == 1){ // Packet from Mgmt module
        handleMgmtInfo(packet);
        return;
    }
    RtwtUMac::handleLowerPacket(packet);
}

void RtwtUMacAccessPoint::sendTwtAgreementsBeacon(uint8_t interfaceId) {
    if(simTime() < par("twtBeaconStart").doubleValue() || simTime() > par("twtBeaconStart").doubleValue() + 0.5){
        return;
    }
    auto interfaceIt = twtAgreementsByInterface.find(interfaceId);

    if (interfaceIt == twtAgreementsByInterface.end()) {
        EV_INFO << "No TWT agreements found for interface " << (int)interfaceId << endl;
        return;
    }
    
    const auto& staMap = interfaceIt->second;
    
    // Look for broadcast agreement (should only be one flow)
    auto broadcastIt = staMap.find(MacAddress::BROADCAST_ADDRESS);
    if (broadcastIt == staMap.end() || broadcastIt->second.empty()) {
        EV_INFO << "No broadcast TWT agreement found for interface " << (int)interfaceId << endl;
        return;
    }
    
    // Get the first (and should be only) flow for broadcast
    const auto& agreement = broadcastIt->second.begin()->second;
    
    auto beaconChunk = makeShared<twtScheduleBeacon>();
    beaconChunk->setScheduleEntriesArraySize(1);
    
    twtScheduleEntry entry;
    entry.setStaAddress(MacAddress::BROADCAST_ADDRESS);
    entry.setWakeTime(agreement.wakeTime);
    entry.setWakeDuration(agreement.wakeDuration);
    entry.setIntervalMantissa(agreement.intervalMantissa);
    entry.setIntervalExponent(agreement.intervalExponent);
    entry.setRestriction(agreement.restriction);

    beaconChunk->setScheduleEntries(0, entry);
    beaconChunk->setChunkLength(B(5+19*1)); // Only one entry for broadcast

    Packet* packet = new Packet("TWT_Beacon");
    packet->addTag<PacketProtocolTag>()->setProtocol(ieee80211beRtwt);
    packet->insertAtBack(beaconChunk);

    int ifaceId = interfaceId;
    if(ifaceId >= 100) {
        ifaceId -= 100;
    }

    NetworkInterface *targetIft = ift->getInterface(ifaceId);
    if(targetIft && targetIft->getState() == NetworkInterface::State::UP) {
        packet->addTagIfAbsent<InterfaceReq>()->setInterfaceId(targetIft->getInterfaceId());
            send(packet, lowerLayerOutGateId);
    }
}

void RtwtUMacAccessPoint::handleApTwtTimer(cMessage *msg) {
    // Extract necessary values from message parameters
    uint8_t interfaceId = msg->par("interfaceId");
    uint8_t flowId = msg->par("flowId"); // Retrieve flow ID from message parameter
    std::string macStr = msg->par("macAddr").stringValue();
    MacAddress staAddress(macStr.c_str());
    if (strncmp(msg->getName(), "twt-wakeup-FF-", 14) == 0){
        sendTwtAgreementsBeacon(interfaceId);
    }

    if (strncmp(msg->getName(), "twt-wakeup-", 11) == 0) {
        TwtUMacAgreement& agreement = twtAgreementsByInterface[interfaceId][staAddress][flowId];
        EV_INFO << "TWT wakeup for STA " << staAddress << ", flowId "
                << (int)flowId << " on interface " << (int)interfaceId
                << " at " << simTime() << endl;
        agreement.isAwake = true;
        // Schedule sleep after wakeDuration
        scheduleAfter(TwtTimeConverter::calculateWakeDuration(agreement.wakeDuration), agreement.sleepMsg);

        // Schedule next wakeup if interval > 0
        if (agreement.intervalMantissa > 0) {
            scheduleAfter(TwtTimeConverter::calculateTwtInterval(agreement.intervalMantissa, agreement.intervalExponent), agreement.wakeupMsg);
        }
    } else if (strncmp(msg->getName(), "twt-sleep-", 10) == 0) {
        TwtUMacAgreement& agreement = twtAgreementsByInterface[interfaceId][staAddress][flowId];
        agreement.isAwake = false;
        EV_INFO << "TWT sleep for STA " << staAddress << ", flowId "
                << (int)flowId << " on interface " << (int)interfaceId
                << " at " << simTime() << endl;
    }
}

void RtwtUMacAccessPoint::handleTwtPacket(Packet* packet){
    if(strcmp(packet->getName(), "twt-requirements") == 0){
        handleTwtLinkRequirements(packet);
    } else if(strcmp(packet->getName(), "beaconInfo") == 0){
        handleBeaconTimers(packet);
    }
}

void RtwtUMacAccessPoint::handleBeaconTimers(Packet* packet){
    // create TWT SP for the Beacon
    const auto& chunk = packet->peekAtFront<beaconTimers>();
    int interfaceId = packet->getTag<InterfaceInd>()->getInterfaceId();
    simtime_t startTime = chunk->getStartTime();
    simtime_t beaconInterval = chunk->getBeaconInterval();

    EV_INFO << "Got Beacon Timer from Iface " << interfaceId << " with start Time: " << startTime
            << " and interval " << beaconInterval << endl;

    // Convert simtime_t to microseconds for the scheduler
    long start_us = startTime.inUnit(SIMTIME_US);
    int duration_us = 2 * 256; // Standard beacon TWT duration: 2 TWT units of 256us each
    long interval_us = beaconInterval.inUnit(SIMTIME_US);    

    auto result = scheduler->forceSchedule(start_us, duration_us, interfaceId);
    scheduleTwtAgreement(interfaceId, MacAddress::BROADCAST_ADDRESS, result.senderFlowIds[0], result.senderWakeTimes[0], result.wakeDurationSlots, result.intervalExponent, result.intervalMantissa);

    delete packet;
}

void RtwtUMacAccessPoint::scheduleTwtAgreement(uint8_t interfaceId, const MacAddress& staAddress,
                               uint8_t flowId, simtime_t wakeTime,
                               uint32_t wakeDuration, uint8_t intervalExponent,
                               uint16_t intervalMantissa)
{
    // Create agreement object directly in the map structure
    TwtUMacAgreement& agreement = twtAgreementsByInterface[interfaceId][staAddress][flowId];

    // Set agreement parameters
    agreement.wakeTime = wakeTime;
    agreement.wakeDuration = wakeDuration;
    agreement.intervalExponent = intervalExponent;
    agreement.intervalMantissa = intervalMantissa;
    agreement.flowId = flowId;
    agreement.interfaceId = interfaceId;
    agreement.staAddress = staAddress;

    // Create messages with proper ownership
    agreement.wakeupMsg = new cMessage(("twt-wakeup-" + staAddress.str() + "-" + std::to_string(flowId)).c_str());
    agreement.sleepMsg = new cMessage(("twt-sleep-" + staAddress.str() + "-" + std::to_string(flowId)).c_str());

    // Configure message parameters using type-safe methods
    agreement.wakeupMsg->addPar("macAddr").setStringValue(staAddress.str().c_str());
    agreement.wakeupMsg->addPar("interfaceId").setLongValue(interfaceId);
    agreement.wakeupMsg->addPar("flowId").setLongValue(flowId);

    agreement.sleepMsg->addPar("macAddr").setStringValue(staAddress.str().c_str());
    agreement.sleepMsg->addPar("interfaceId").setLongValue(interfaceId);
    agreement.sleepMsg->addPar("flowId").setLongValue(flowId);

    // Schedule events
    EV_INFO << "TWT scheduled for STA " << staAddress << ", flowId " << static_cast<int>(flowId)
            << ". First wakeup at " << wakeTime << endl;
    scheduleAt(wakeTime, agreement.wakeupMsg);

    EV_INFO << "Added agreement for interface " << static_cast<int>(interfaceId)
            << ". Map size: " << twtAgreementsByInterface[interfaceId].size() << endl;
}

void RtwtUMacAccessPoint::handleTwtLinkRequirements(Packet* packet){
    scheduler->handleTwtLinkRequest(packet);  //Requests are handled by the scheduler
    delete packet;
}

void RtwtUMacAccessPoint::sendTwtResponse(Ptr<ieee80211::twtFrame> frame){
    Packet *pkt = new Packet("twt-response");
    pkt->addTag<PacketProtocolTag>()->setProtocol(ieee80211beRtwt);
    pkt->setKind(0); //Response
    pkt->insertAtBack(frame);
    int interfaceId = frame->getInterfaceId();
    if(interfaceId>=100){
        interfaceId -=100;
    }
    NetworkInterface *targetIft = ift->getInterface(interfaceId);
    if(targetIft->getState() == NetworkInterface::State::UP) {
        pkt->addTagIfAbsent<InterfaceReq>()->setInterfaceId(targetIft->getInterfaceId());
        send(pkt, lowerLayerOutGateId);
    }
}

void RtwtUMacAccessPoint::scheduleAgreementAndSendResponse(uint8_t interfaceId, const MacAddress& staAddress,uint8_t flowId, simtime_t wakeTime, uint32_t wakeDuration, uint8_t intervalExponent, uint16_t intervalMantissa) {
    //called by Scheduler
    scheduleTwtAgreement(interfaceId, staAddress, flowId, wakeTime, wakeDuration, intervalExponent, intervalMantissa);
    
    // Create and send TWT response frame
    auto responseFrame = scheduler->createTwtResponseFrame(flowId, 0, interfaceId, wakeTime, wakeDuration,intervalMantissa, intervalExponent, staAddress);
    sendTwtResponse(responseFrame);
}

void RtwtUMacAccessPoint::handleMgmtInfo(Packet *packet){  //handle association information from Mgmt module
    const auto& chunk = packet->peekAtFront<ManagementInfoFrame>();
    MacAddress staAddress = chunk->getStaAddress();
    uint8_t interfaceId = chunk->getInterfaceId();
    uint8_t status = chunk->getStatus();
    interfaceAssociations[interfaceId].insert(staAddress); //Handle only associations
    EV_INFO << "UMac received Association Information from: " << staAddress << " with InterfaceId " <<  interfaceId << " and status " << status << endl;
}


}
