//
//  Created on: April 2025
//      Author: Mateusz Zakrzewski, Doganalp Ergenc (TU Berlin)
//
//
// R-TWT enabled STA module. Its L-MAC is enhanced with R-TWT agent and the management module can forward the beacons to U-MAC to leverage the topology information.
// It is an extension of MLOStation from the base MLO framework (which simply extends 802.11 station in INET with MLO support).
//

package rtwt_mlo.node;

import mlo.node.MLOStation;
import rtwt_mlo.umac.RtwtUMac;

module RtwtStation extends MLOStation
{
    parameters:
        // MODIFICATION BEGIN: R-TWT support
        lmac[*].mgmt.typename = default("Ieee80211LMgmtSta");
        umac.typename = "RtwtUMac";
        lmac[*].typename = "Ieee80211LInterfaceRtwt";
        // MODIFICATION END
}

