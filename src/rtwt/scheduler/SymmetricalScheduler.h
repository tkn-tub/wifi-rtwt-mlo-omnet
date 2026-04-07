//
//  Created on: April 2025
//      Author: Mateusz Zakrzewski, Doganalp Ergenc (TU Berlin)
//

// The symmetrical scheduler implementation. It divides the calculated service periods equally (into two) and allocates them over two links.
// It resembles with the link aggregation, e.g., the SP is actualized in parallel over two links.
//


#ifndef UMAC_SCHEDULER_SYMMETRICALSCHEDULER_H_
#define UMAC_SCHEDULER_SYMMETRICALSCHEDULER_H_

#include <omnetpp.h>
#include "inet/common/packet/Packet.h"

#include "src/rtwt/msg/twtFrame_m.h"
#include "TwtSchedulerBase.h"

namespace inet {
using namespace omnetpp;

class SymmetricalScheduler : public TwtSchedulerBase
{
    public:
        virtual void scheduleRequestsWithSlots(const std::vector<double>& requestSlots) override;
};

}
#endif /* UMAC_SCHEDULER_SYMMETRICALSCHEDULER_H_ */

