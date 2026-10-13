//
//  Created on: April 2025
//      Author: Mateusz Zakrzewski, Doganalp Ergenc (TU Berlin)
//
//
// TWT agent coordinates radio sleep/wake oeprations under the control of U-MAC.
//

#include <omnetpp.h>


#include "inet/common/Protocol.h"
#include "inet/networklayer/common/NetworkInterface.h"
#include "inet/common/ModuleAccess.h"
#include "inet/common/InitStages.h"
#include "inet/common/INETUtils.h"
#include "inet/linklayer/common/MacAddressTag_m.h"
#include "inet/linklayer/common/InterfaceTag_m.h"
#include "inet/common/IInterfaceRegistrationListener.h"
#include "inet/linklayer/ieee80211/mgmt/Ieee80211MgmtSta.h"
#include "inet/common/INETUtils.h"
#include "inet/common/ModuleAccess.h"
#include "inet/common/Simsignals.h"
#include "inet/common/packet/Message.h"
#include "inet/linklayer/ieee80211/mac/Ieee80211SubtypeTag_m.h"
#include "inet/networklayer/common/NetworkInterface.h"
#include "inet/physicallayer/wireless/common/contract/packetlevel/IRadioMedium.h"
#include "inet/physicallayer/wireless/common/contract/packetlevel/RadioControlInfo_m.h"
#include "inet/physicallayer/wireless/common/contract/packetlevel/SignalTag_m.h"
#include "inet/physicallayer/wireless/ieee80211/packetlevel/Ieee80211ControlInfo_m.h"
#include "inet/networklayer/contract/IInterfaceTable.h"
#include "inet/linklayer/common/MacAddress.h"
#include "inet/common/IInterfaceRegistrationListener.h"

#include "TwtAgent.h"

#include "../../lmac/mac/Ieee80211LMacRtwt.h"
#include "src/lmac/mac/Ieee80211beRtwtProtocol.h"
#include "src/rtwt/TwtTimeConverter.h"
#include "src/rtwt/msg/twtFrame_m.h"
#include "src/rtwt/msg/twtScheduleBeacon_m.h"
#include "src/rtwt/msg/signalRadio_m.h"
#include "src/rtwt/msg/twtLinkRequirement_m.h"

namespace inet {
namespace ieee80211 {

Register_Class(TwtAgent);

int TwtAgent::numInitStages() const {
    return 13; // enable more Init stages
}

void TwtAgent::initialize(int stage)
{
    EV_INFO << "TWT Agent Initialize Stage: " << stage << endl;
    if (stage == INITSTAGE_LOCAL) { 
        isStation = par("isStation");
        scheduleBuffer = par("scheduleBuffer");
    } else if(stage == INITSTAGE_LINK_LAYER){ //stage 9
        registerProtocol(*ieee80211beRtwt, gate("macIn"), gate("macOut"));
    } else if (stage == INITSTAGE_NETWORK_CONFIGURATION){ //stage 12
        auto macModule = getParentModule()->getSubmodule("mac");
        if (macModule) {
            auto mac = check_and_cast<Ieee80211Mac*>(macModule);
            myMac = mac->getAddress();
            EV_INFO << "My MAC: " << myMac << endl;
        }
    }
}

void TwtAgent::handleMessage(cMessage *msg){
    if (msg->isSelfMessage()){
//        Process TWT Timer messages
        handleSelfMessage(msg);
    } else if (msg->arrivedOn("macIn")){
//        Process message from Radio
        processIncomingFrame(check_and_cast<Packet*>(msg));
    } else if (msg->arrivedOn("llcIn")){
//        Process message from UMac
        handleUmacMessage(check_and_cast<Packet*>(msg));
    } else {
        delete msg;
    }
}

void TwtAgent::handleSelfMessage(cMessage *msg){    //Handle TWT Timers
    if (strncmp(msg->getName(), "twt-wakeup-", 11) == 0) {
        MacAddress *staAddress = static_cast<MacAddress*>(msg->getContextPointer());
        uint8_t flowId = msg->getKind();

        auto staIt = twtAgreements.find(*staAddress);
        if (staIt != twtAgreements.end()) {
            auto flowIt = staIt->second.find(flowId);
            if (flowIt != staIt->second.end()) {
                TwtAgreement& agreement = flowIt->second;

                EV_INFO << "TWT wakeup for STA " << *staAddress << ", flowId "
                        << (int)flowId << " at " << simTime() << endl;

                if (agreement.wakeupMsg && agreement.wakeupMsg->isScheduled()) {
                    cancelEvent(agreement.wakeupMsg);
                }
                if (agreement.sleepMsg && agreement.sleepMsg->isScheduled()) {
                    cancelEvent(agreement.sleepMsg);
                }                // Schedule sleep after wakeDuration
                scheduleAfter(agreement.wakeDuration, agreement.sleepMsg);                // Send next Sleep time to radio
                signalToRadio(1,0); // 1 for wakeup, 0 for no offset
                signalToLMac(1); // Signal wake state to LMac

                if (agreement.interval > SIMTIME_ZERO) {
                    scheduleAfter(agreement.interval, agreement.wakeupMsg);
                }
            }
        }
    }    else if (strncmp(msg->getName(), "twt-sleep-", 10) == 0) {
        MacAddress *staAddress = static_cast<MacAddress*>(msg->getContextPointer());
        uint8_t flowId = msg->getKind();
        lastTwtSpMac =  *staAddress;
        EV_INFO << "TWT sleep for STA " << staAddress << ", flowId " << (int)flowId << " at " << simTime() << endl;
        // Signal sleep state to LMac
        signalToLMac(0);
        signalToRadio(0,scheduleBuffer);
    }
}

void TwtAgent::handleUmacMessage(Packet *packet){   //Handle messages from UMac downstream
    if(strcmp(packet->getName(), "twt-requirements") == 0){ //Forward requests to other node (AP)
        const auto& chunk = packet->peekData();
        if(auto twtChunk = dynamicPtrCast<const twtLinkRequirement>(chunk)){
            packet->addTagIfAbsent<Ieee80211SubtypeReq>()->setSubtype(ST_BEACON);
            send(packet, "macOut");
            return;
        }
    } else if(strcmp(packet->getName(), "TWT_Beacon") == 0){    //Send TWT Beacons to broadcast in MAC
        const auto& chunk = packet->peekData();
        if(auto twtChunk = dynamicPtrCast<const twtScheduleBeacon>(chunk)){
            packet->addTagIfAbsent<Ieee80211SubtypeReq>()->setSubtype(ST_BEACON);
            send(packet, "macOut");
            return;
        }
    }
    const auto& frame = packet->peekAtFront<twtFrame>();
    if (frame != nullptr) {
        if(frame->getType() == 0){
            EV_INFO << "Received TWT Response from UMac. Adding to schedule and forwarding the response." << endl;

            simtime_t interval = TwtTimeConverter::calculateTwtInterval(frame->getIntervalMantissa(), frame->getIntervalExponent());
            simtime_t wakeDuration = TwtTimeConverter::calculateWakeDuration(frame->getWakeDuration());

            scheduleWakeup(frame->getSTAaddress(), frame->getFlowId() , frame->getWakeTime(), wakeDuration, interval);
        } else {
            EV_INFO << "Received TWT Message from UMac. Forwarding the message." << endl;
        }

        packet->addTagIfAbsent<Ieee80211SubtypeReq>()->setSubtype(ST_BEACON);
        send(packet, "macOut");
    }
}

void TwtAgent::handleTwtBeacon(Packet *packet){ //Handle received TWT Beacon in STA, could handle multiple TWT Agreements
    const auto& beaconChunk = packet->peekAtFront<twtScheduleBeacon>();
    size_t entryCount = beaconChunk->getScheduleEntriesArraySize();

    for (size_t i = 0; i < entryCount; i++) {
        const auto& entry = beaconChunk->getScheduleEntries(i);
        MacAddress staAddress = entry.getStaAddress();

        // Skip entries not relevant to this station
        if (staAddress != MacAddress::BROADCAST_ADDRESS && staAddress != myMac) {
            continue;
        }
        // Process relevant entry
        simtime_t wakeTime = entry.getWakeTime();
        uint32_t wakeDuration = entry.getWakeDuration();
        uint16_t intervalMantissa = entry.getIntervalMantissa();
        uint8_t intervalExponent = entry.getIntervalExponent();
        uint8_t restriction = entry.getRestriction();

        // Determine flow ID based on whether this is a broadcast/individual/group agreement
        uint8_t flowId = 0;  // Default for broadcast
        if (staAddress != MacAddress::BROADCAST_ADDRESS) {
            if(restriction != 0){
                flowId = restriction;   //assign restriction group as flow ID
            }
        }

        auto staIt = twtAgreements.find(staAddress);
        if (staIt != twtAgreements.end()) {
            EV_INFO << "TWT Agreement from Beacon already exists" << endl;
        } else {
            simtime_t interval = TwtTimeConverter::calculateTwtInterval(intervalMantissa, intervalExponent);
            scheduleWakeup(staAddress,flowId, ((int)(simTime()/interval)+1)*interval+wakeTime, TwtTimeConverter::calculateWakeDuration(wakeDuration), interval);
        }
    }
}

void TwtAgent::processIncomingFrame(Packet *packet) {   //Select correct handlers for messages from Radio
    if(strcmp(packet->getName(), "twt-requirements") == 0){
        forwardToUMac(packet);
        return;
    } else if(strcmp(packet->getName(), "TWT_Beacon") == 0){
//      Handle TWT Schedule Beacons
        handleTwtBeacon(packet);
        delete packet;
        return;
    }
    const auto& chunk = packet->peekAtFront<twtFrame>();

    if (chunk != nullptr) {
        EV_INFO << "Received TWT frame type: " << (int)chunk->getType() << endl;
        switch (chunk->getType()) {
            case 0:  // Response
                processTwtResponse(packet);
                break;
            case 1:  // Request
                processTwtRequest(packet);
                break;
            case 2:  // Teardown
                // not implemented
                break;
            default:
                EV_ERROR << "Unknown TWT frame type"<< endl;
                delete packet;
        }
    } else {
        delete packet;
    }
}

void TwtAgent::processTwtRequest(Packet *packet) { // Recieved TWT Request from radio always forward to UMac
    forwardToUMac(packet);
}

void TwtAgent::forwardToUMac(Packet *packet) {
    packet->addTagIfAbsent<MacProtocolInd>()->setProtocol(ieee80211beRtwt);
    send(packet, "llcOut");
}

void TwtAgent::processTwtResponse(Packet* packet) { //save TWT Response from AP in the schedule
    const auto& frame = packet->peekAtFront<twtFrame>();
    EV_INFO << "Processing TWT Response" << endl;
    simtime_t interval = TwtTimeConverter::calculateTwtInterval(frame->getIntervalMantissa(), frame->getIntervalExponent());
    simtime_t wakeDuration = TwtTimeConverter::calculateWakeDuration(frame->getWakeDuration());

    scheduleWakeup(frame->getSTAaddress(), frame->getFlowId() , frame->getWakeTime(), wakeDuration, interval);
}

void TwtAgent::scheduleWakeup(const MacAddress& staAddress, uint8_t flowId, simtime_t wakeTime, simtime_t wakeDuration, simtime_t interval) { //Save SP Parameters in schedule
    EV_INFO << "Scheduling wakeup at " << wakeTime << " for " << wakeDuration << " every " << interval << endl;

    auto staIt = twtAgreements.find(staAddress);
    std::map<uint8_t, TwtAgreement> flowMap;
    TwtAgreement agreement;
    agreement.wakeTime = wakeTime;
    agreement.wakeDuration = wakeDuration;
    agreement.interval = interval;
    agreement.flowId = flowId;
    agreement.isActive = true;
    agreement.wakeupMsg = new cMessage(("twt-wakeup-" + staAddress.str() + "-" + std::to_string(flowId)).c_str());
    agreement.sleepMsg = new cMessage(("twt-sleep-" + staAddress.str() + "-" + std::to_string(flowId)).c_str());

    // Store the agreement
    flowMap[flowId] = agreement;
    twtAgreements[staAddress] = flowMap;

    simtime_t firstWakeup = wakeTime;
    agreement.wakeupMsg->setContextPointer(new MacAddress(staAddress));
    agreement.wakeupMsg->setKind(flowId);  // Use kind field to store flowId
    agreement.sleepMsg->setContextPointer(new MacAddress(staAddress));
    agreement.sleepMsg->setKind(flowId);    
    scheduleAt(firstWakeup, agreement.wakeupMsg);
    EV_INFO << "TWT scheduled for STA " << staAddress << ", flowId " << (int)flowId
            << ". First wakeup at " << firstWakeup << endl;
}

void TwtAgent::signalToRadio(uint8_t type, uint32_t offset) {   //Send signal to Radio only in STA
    if(isStation){
        Packet *pkt = new Packet("radioSignalChange");
        const auto& body = makeShared<signalRadio>();
        body->setType(type);
        body->setOffset(offset*256);
        body->setChunkLength(B(5));
        pkt->insertAtBack(body);
        send(pkt, "radioOut");
    }
}

void TwtAgent::signalToLMac(uint8_t type) {
    Packet *pkt;
    if(type == 0) {
        pkt = new Packet("twtSleep");
        EV_INFO << "TWT sleep signal sent to LMac at " << simTime() << endl;
    } else {
        pkt = new Packet("twtWakeUp");
        EV_INFO << "TWT wake signal sent to LMac at " << simTime() << endl;
    }
    send(pkt, "macOut");
}

int TwtAgent::activeTwtSp() {   //Function for checking active TWT SP
    simtime_t now = simTime();
    if(now < 5){    //During setup always allow to send
        return 1;
    }
    // First check for broadcast SPs (higher priority)
    for (auto staIt = twtAgreements.begin(); staIt != twtAgreements.end(); ++staIt) {
        const MacAddress& staAddr = staIt->first;
        if (staAddr == MacAddress::BROADCAST_ADDRESS) {
            for (auto flowIt = staIt->second.begin(); flowIt != staIt->second.end(); ++flowIt) {
                const TwtAgreement& agreement = flowIt->second;
                if (agreement.isActive && agreement.sleepMsg && agreement.sleepMsg->isScheduled()) {
                    // A scheduled sleep message indicates the station is currently in its SP
                    return 1; // Broadcast SP is active
                }
            }
        } else {
            for (auto flowIt = staIt->second.begin(); flowIt != staIt->second.end(); ++flowIt) {
                const TwtAgreement& agreement = flowIt->second;
                if (agreement.isActive && agreement.sleepMsg && agreement.sleepMsg->isScheduled()) {
                    // A scheduled sleep message indicates the station is currently in its SP
                    return 2; // Individual SP is active
                }
            }
        }
    }

    // No SP is active
    return -1;
}

MacAddress TwtAgent::activeTwtSpMacAddr() { //Get MAC Adress of currently active TWT SP
    simtime_t now = simTime();
    if(now < 5){    //During setup always allow to send
        return MacAddress::UNSPECIFIED_ADDRESS;
    }
    // First check for broadcast SPs (higher priority)
    for (auto staIt = twtAgreements.begin(); staIt != twtAgreements.end(); ++staIt) {
        const MacAddress& staAddr = staIt->first;
        if (staAddr == MacAddress::BROADCAST_ADDRESS) {
            for (auto flowIt = staIt->second.begin(); flowIt != staIt->second.end(); ++flowIt) {
                const TwtAgreement& agreement = flowIt->second;
                if (agreement.isActive && agreement.sleepMsg && agreement.sleepMsg->isScheduled()) {
                    // A scheduled sleep message indicates the station is currently in its SP
                    return staAddr;
                }
            }
        } else {
            for (auto flowIt = staIt->second.begin(); flowIt != staIt->second.end(); ++flowIt) {
                const TwtAgreement& agreement = flowIt->second;
                if (agreement.isActive && agreement.sleepMsg && agreement.sleepMsg->isScheduled()) {
                    // A scheduled sleep message indicates the station is currently in its SP
                    return staAddr;
                }
            }
        }
    }

    // No SP is active
    return MacAddress::UNSPECIFIED_ADDRESS;
}

MacAddress TwtAgent::getLastTwtSpMac(){
    return lastTwtSpMac;
}

}
}
