//
//  Created on: April 2025
//      Author: Mateusz Zakrzewski (TU Berlin)
//
// This module extends the ACK management according to the R-TWT SP management.
// It keeps track of the block ACKs per destination STA, which are supposed to be sent at the end of their specific SPs.
//

package rtwt_mlo.lmac.mac.originator;

import inet.linklayer.ieee80211.mac.originator.QosAckHandler;

simple QosTwtAckHandler extends QosAckHandler
{
    parameters:
        @class(inet::ieee80211::QosTwtAckHandler);
        @display("i=block/table2");
}