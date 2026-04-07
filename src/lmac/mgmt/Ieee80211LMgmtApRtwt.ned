//
//  Created on: April 2025
//      Author: Mateusz Zakrzewski, Doganalp Ergenc (TU Berlin)
//
// The management module for R-TWT-enabled AP. 
// The AP should send the beacons when the a STA is awake - thus, the first slots of every SP is dedicated for beacons.
// This module accordingly configures the AP management routines to make sure that all STAs get the necessary beacons when they are awake.
//

package rtwt_mlo.lmac.mgmt;

import mlo.lmac.mgmt.IIeee80211LMgmt;

import inet.linklayer.ieee80211.mgmt.Ieee80211MgmtAp;

module Ieee80211LMgmtApRtwt extends Ieee80211MgmtAp like IIeee80211LMgmt{
    parameters:
        @class(inet::ieee80211::Ieee80211LMgmtApRtwt);
        bool twtEnable = default(false);
	gates:
        output llcOut;
        input llcIn @loose;
}