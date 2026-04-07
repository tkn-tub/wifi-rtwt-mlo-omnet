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

// U-MAC module for R-TWT enabled STA (generic). 
// - It extends the base U-MAC implementation from the MLO project (Umac).
// - STAs announce their link requirements provided in the .ini file.
// - SLO mode is also available.

package rtwt_mlo.umac;

import mlo.umac.UMac;
import mlo.umac.IUMac;
import inet.linklayer.contract.ILinkLayer;

module RtwtUMac extends UMac like IUMac
{
    parameters:
        @class(inet::RtwtUMac);
        object twtParameters = default([]);
        object linkRequirements = default([]);
        bool twtEnable = default(false);
        int sloLink = default(-1);
        mode = default("RtwtUMac");
}
