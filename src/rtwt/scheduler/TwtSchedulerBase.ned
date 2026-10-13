//
//  Created on: April 2025
//      Author: Mateusz Zakrzewski, Doganalp Ergenc (TU Berlin)
//

// The base scheduler implementation. It mainly handles:
// - the collection and processing of TWT link requests
// - calculation of necessary time slots for every request (using max-min fairness).
// - creating TWT responses.
// It is specifically implemented for 2-link MLO. The distribution of the service periods over multiple links is not handled in this base module.


package rtwt_mlo.rtwt.scheduler;

import rtwt_mlo.rtwt.scheduler.IScheduler;

module TwtSchedulerBase like IScheduler
{
    parameters:
        @class(inet::TwtSchedulerBase);
        int minimalInterval = default(256);
        int scheduleBuffer = default(2);  // Buffer between consecutive schedules in 256us units
        double cycleDuration @unit(ms) = default(128ms); //later converted to 128*1024us
        double practicalBitrate @unit(bps);
        double dataOverhead;
}
