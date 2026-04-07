//
//  Created on: April 2025
//      Author: Mateusz Zakrzewski, Doganalp Ergenc (TU Berlin)
//
// This is fundamentally the same with the LEdca in the base MLO framework (which is an extension of 802.11 Edca in INET). Only the EDCAF module is changed.
// EDCAF module is not flexibly configured - thus we have this mostly duplicate module.
//

package rtwt_mlo.lmac.mac.channelaccess;

import inet.linklayer.ieee80211.mac.contract.ICollisionController;
import inet.linklayer.ieee80211.mac.originator.NonQosRecoveryProcedure;

module LEdcaRtwt
{
    parameters:
		@class(inet::ieee80211::LEdcaRtwt);
        int numEdcafs = default(4);
        string rxModule;
        string rateSelectionModule;

        *.rateSelectionModule = "^." + this.rateSelectionModule;
        *.rxModule = "^." + this.rxModule;
        *.collisionControllerModule = "^.collisionController";

        edcaf[0].accessCategory = default("AC_BK");
        edcaf[1].accessCategory = default("AC_BE");
        edcaf[2].accessCategory = default("AC_VI");
        edcaf[3].accessCategory = default("AC_VO");
        
        @display("i=block/star");
    submodules:
        edcaf[numEdcafs]: LEdcafRtwt {
            parameters:
                @display("p=100,100,row,150");
        }
        collisionController: <default("EdcaCollisionController")> like ICollisionController {
            parameters:
                @display("p=100,200");
        }
        mgmtAndNonQoSRecoveryProcedure: NonQosRecoveryProcedure {
            parameters:
                cwCalculatorModule = "^.edcaf[1]"; // best effort
                rtsPolicyModule = "^.^.rtsPolicy";
                @display("p=400,200");
        }
}


