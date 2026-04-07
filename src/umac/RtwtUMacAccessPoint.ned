//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

//
//  Created on: April 2025
//      Author: Mateusz Zakrzewski, Doganalp Ergenc (TU Berlin)
//

// U-MAC module for R-TWT enabled AP. 
// - It extends the base R-TWT enabled U-MAC implementation (RtwtUmac).
// - It has a special queueing facility to orchestrate transmissions within the wake times of the destination STAs.
// - To be able to use R-TWT, it should deploy a scheduler. Symmetrical, asymmetrical, and cross-symmetrical schedulers are available.

package rtwt_mlo.umac;

import rtwt_mlo.rtwt.scheduler.IScheduler;

import mlo.umac.IUMac;

module RtwtUMacAccessPoint extends RtwtUMac like IUMac
{
    parameters:
        @class(inet::RtwtUMacAccessPoint);
        mode = "RtwtUMacAccessPoint";
        double twtBeaconStart = default(3.5);
	submodules:
	    twtScheduler: <default("SymmetricalScheduler")> like IScheduler  { }
}
