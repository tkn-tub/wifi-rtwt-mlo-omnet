//
//  Created on: April 2025
//      Author: Mateusz Zakrzewski
//
// The implementation of wake-up radio under the control of our R-TWT agent. It supports periodical wakes/sleeps according to the R-TWT schedules.
//

#include "Ieee80211ahTwtRadio.h"
#include "inet/common/ModuleAccess.h"
#include "inet/linklayer/common/MacAddressTag_m.h"
#include "inet/physicallayer/wireless/common/radio/packetlevel/Radio.h"
#include "inet/physicallayer/wireless/common/medium/RadioMedium.h"
#include "inet/common/Simsignals.h"
#include "src/rtwt/msg/signalRadio_m.h"
#include "inet/common/packet/Packet.h"
#include "inet/common/packet/Message_m.h"
#include <omnetpp.h>

namespace inet {

namespace physicallayer {

Define_Module(Ieee80211ahTwtRadio);

void Ieee80211ahTwtRadio::initialize(int stage)
{
    Ieee80211Radio::initialize(stage);
}

void Ieee80211ahTwtRadio::handleMessage(cMessage *message)
{
    if (message->arrivedOn("twtAgentIn")) {
        handleTwtMessage(check_and_cast<Packet *>(message));
        return;
    }
    else if(message->isSelfMessage() && isTwtTimer(message)){
        handleTwtTimer(message);
        return;
    }
    //Process all other messages as usual
    Ieee80211Radio::handleMessage(message);
}

bool Ieee80211ahTwtRadio::isTwtTimer(cMessage *message){
    if(strncmp(message->getName(), "wakeUp", 6) == 0 || strncmp(message->getName(), "sleep", 5) == 0){
        return true;
    }
    return false;
}

void Ieee80211ahTwtRadio::handleTwtTimer(cMessage *message){
    if (strncmp(message->getName(), "wakeUp", 6) == 0) {
        if (radioMode == RADIO_MODE_SLEEP || radioMode == RADIO_MODE_OFF) {
            setRadioMode(RADIO_MODE_RECEIVER);
            EV_INFO << "Change radio state to awake." << endl;
        }
    } else if(strncmp(message->getName(), "sleep", 5) == 0) {
        if (radioMode != RADIO_MODE_SLEEP && radioMode != RADIO_MODE_OFF) {
            setRadioMode(RADIO_MODE_SLEEP);
            EV_INFO << "Change radio state to sleep." << endl;
        }
    }
    delete message;
}

void Ieee80211ahTwtRadio::handleTwtMessage(Packet *packet) {    //handle Wakeup/Sleep TWT messages with offset
    const auto& chunk = packet->peekAtFront<ieee80211::signalRadio>();
    if(chunk->getType() == 1){ // Wakeup
        if(chunk->getOffset() == 0){
            if (radioMode == RADIO_MODE_SLEEP || radioMode == RADIO_MODE_OFF) {
                setRadioMode(RADIO_MODE_RECEIVER);
                EV_INFO << "Change radio state to awake." << endl;
            }
        } else {
            cMessage *msg = new cMessage("wakeUp");
            scheduleAfter(SimTime(chunk->getOffset(), SIMTIME_US), msg);
        }
    } else if(chunk->getType() == 0){ // Sleep
        if(chunk->getOffset() == 0){
            if (radioMode != RADIO_MODE_SLEEP && radioMode != RADIO_MODE_OFF) {
                setRadioMode(RADIO_MODE_SLEEP);
                EV_INFO << "Change radio state to sleep." << endl;
            }
        } else {
            cMessage *msg = new cMessage("sleep");
            scheduleAfter(SimTime(chunk->getOffset(), SIMTIME_US), msg);
        }

    } else {
        EV_ERROR << "Unknown TWT frame type"<< endl;
    }
    delete packet;
}

}
}
