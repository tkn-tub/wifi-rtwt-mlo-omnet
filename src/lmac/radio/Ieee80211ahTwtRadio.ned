//
//  Created on: April 2025
//      Author: Mateusz Zakrzewski
//
// The implementation of wake-up radio under the control of our R-TWT agent. It supports periodical wakes/sleeps according to the R-TWT schedules.
//

package rtwt_mlo.lmac.radio;

import inet.physicallayer.wireless.ieee80211.packetlevel.Ieee80211ScalarRadio;


module Ieee80211ahTwtRadio extends Ieee80211ScalarRadio
{
    parameters: 
        @class(inet::physicallayer::Ieee80211ahTwtRadio);
	gates:
	    input twtAgentIn;
}
