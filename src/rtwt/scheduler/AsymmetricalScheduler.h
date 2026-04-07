//
//  Created on: April 2025
//      Author: Mateusz Zakrzewski, Doganalp Ergenc (TU Berlin)
//

// The asymmetrical scheduler implementation. It divides the calculated service periods proportional to the data rates at each link.
// Differently than the symmetrical scheduler, it also enables simultaneous transmission and reception.

#ifndef UMAC_SCHEDULER_ASYMMETRICALSCHEDULER_H_
#define UMAC_SCHEDULER_ASYMMETRICALSCHEDULER_H_

#include <omnetpp.h>
#include "inet/common/packet/Packet.h"

#include "src/rtwt/msg/twtFrame_m.h"
#include "TwtSchedulerBase.h"

namespace inet {
using namespace omnetpp;

class AsymmetricalScheduler : public TwtSchedulerBase
{
    public:
        virtual void scheduleRequestsWithSlots(const std::vector<double>& requestSlots) override;
        virtual int calculatePurgeSlots(int wakeDurationSlots);
};

}
#endif /* UMAC_SCHEDULER_ASYMMETRICALSCHEDULER_H_ */
