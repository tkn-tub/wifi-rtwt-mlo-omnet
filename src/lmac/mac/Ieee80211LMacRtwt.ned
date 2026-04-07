//
//  Created on: April 2025
//      Author: Mateusz Zakrzewski (TU Berlin)
//
// This module extends the L-MAC interfaces in the base MLO framework (Ieee80211LMac).
// It includes a new R-TWT agent, and uses the extended R-TWT-aware HCF.
//

package rtwt_mlo.lmac.mac;

import inet.linklayer.base.MacProtocolBase;
import inet.linklayer.ieee80211.IIeee80211Mac;

import mlo.lmac.mac.Ieee80211LMac;

//
module Ieee80211LMacRtwt extends Ieee80211LMac like IIeee80211Mac
{
    parameters:
        @class(inet::ieee80211::Ieee80211LMacRtwt);
        bool twtEnable = default(false);
        hcf.typename = "LHcfRtwt";
    gates:
        input twtIn;
        output twtOut;
}

