//
//  Created on: April 2025
//      Author: Mateusz Zakrzewski
//
// The implementation of wake-up radio under the control of our R-TWT agent. It supports periodical wakes/sleeps according to the R-TWT schedules.
//

#ifndef LMAC_RADIO_Ieee80211ahTwtRadio_H_
#define LMAC_RADIO_Ieee80211ahTwtRadio_H_

#include <omnetpp.h>
#include "inet/physicallayer/wireless/ieee80211/packetlevel/Ieee80211Radio.h"
#include "inet/common/packet/Packet.h"
using namespace omnetpp;

namespace inet {
namespace physicallayer {

    class Ieee80211ahTwtRadio : public Ieee80211Radio {
        protected:
            virtual void initialize(int stage) override;
            virtual void handleMessage(cMessage *message) override;
            virtual void handleTwtTimer(cMessage *message);
            virtual void handleTwtMessage(Packet *packet);
            bool isTwtTimer(cMessage *message);
    };

}
}

#endif /* LMAC_RADIO_Ieee80211ahTwtRadio_H_ */
