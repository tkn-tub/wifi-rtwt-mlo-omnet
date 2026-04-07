//
//  Created on: April 2025
//      Author: Mateusz Zakrzewski, Doganalp Ergenc (TU Berlin)
//

// The asymmetrical scheduler implementation. It divides the calculated service periods proportional to the data rates at each link.
// Differently than the symmetrical scheduler, it also enables simultaneous transmission and reception.

package rtwt_mlo.rtwt.scheduler;

import rtwt_mlo.rtwt.scheduler.TwtSchedulerBase;

module AsymmetricalScheduler extends TwtSchedulerBase
{
    parameters:
        @class(inet::AsymmetricalScheduler);
}
