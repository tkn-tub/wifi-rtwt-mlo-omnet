//
//  Created on: April 2025
//      Author: Mateusz Zakrzewski, Doganalp Ergenc (TU Berlin)
//
//
// The symmetrical scheduler implementation. It divides the calculated service periods equally (into two) and allocates them over two links. 
// It resembles with the link aggregation, e.g., the SP is actualized in parallel over two links.
//

package rtwt_mlo.rtwt.scheduler;

import rtwt_mlo.rtwt.scheduler.TwtSchedulerBase;

module SymmetricalScheduler extends TwtSchedulerBase
{
    parameters:
        @class(inet::SymmetricalScheduler);
}
