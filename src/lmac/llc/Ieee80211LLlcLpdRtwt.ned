//
//  Created on: 27.03.2025
//      Author: zakrzewski
//

package rtwt_mlo.lmac.llc;

import mlo.lmac.llc.Ieee80211LLlcLpd;

simple Ieee80211LLlcLpdRtwt extends Ieee80211LLlcLpd 
{
    @class(inet::ieee80211::Ieee80211LLlcLpdRtwt);
    // MODIFICATION: New gates are added to pass R-TWT messages.
    gates: 
        input twtAgentIn;
        output twtAgentOut;
        input mgmtIn;
}
