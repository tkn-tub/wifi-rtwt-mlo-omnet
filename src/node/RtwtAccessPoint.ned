//
//  Created on: April 2025
//      Author: Mateusz Zakrzewski, Doganalp Ergenc (TU Berlin)
//
//
// R-TWT enabled AP module. Its L-MAC is enhanced with R-TWT agent and the management module can forward the beacons to U-MAC to leverage the topology information.
// For R-TWT support, it also needs to employ a scheduler (see rtwt/schedulers).
// It is an extension of MLOAccessPoint from the base MLO framework (which simply extends 802.11 AP in INET with MLO support).
//

package rtwt_mlo.node;

import mlo.node.MLOAccessPoint;
import rtwt_mlo.umac.RtwtUMac;
import rtwt_mlo.umac.RtwtUMacAccessPoint;

module RtwtAccessPoint extends MLOAccessPoint
{
    parameters:
        // MODIFICATION BEGIN: R-TWT scheduler
        lmac[*].typename = default("Ieee80211LInterfaceRtwt");
        lmac[*].mgmt.typename = default("Ieee80211LMgmtApRtwt");
        umac.typename = default("RtwtUMacAccessPoint");
        umac.twtScheduler.typename = default("SymmetricalScheduler");
        // MODIFICATION END
}

