//
//  Created on: April 2025
//      Author: Mateusz Zakrzewski, Doganalp Ergenc (TU Berlin)
//

// The cross-symmetrical scheduler implementation. It divides the calculated service periods equally (similar to the Symmetrical scheduler).
// However, instead of aggregating links, it enables simultaneous tranmission and reception (i.e., sender SP at one link and receiver SP at another link).
// Note that this simultaneous tranmission and reception does NOT mean the STR in MLO context. Here, the tranmission and reception occur at the sender and the receiver MLDs respectively.
// Nevertheless, all MLO operations in our implementation is STR-enabled.

package rtwt_mlo.rtwt.scheduler;

import rtwt_mlo.rtwt.scheduler.TwtSchedulerBase;

module CrossSymmetricalScheduler extends TwtSchedulerBase
{
    parameters:
        @class(inet::CrossSymmetricalScheduler);
}
