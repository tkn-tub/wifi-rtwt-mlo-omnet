//
//  Created on: April 2025
//      Author: Mateusz Zakrzewski, Doganalp Ergenc (TU Berlin)
//
// The management module for R-TWT-enabled AP.
// The AP should send the beacons when the a STA is awake - thus, the first slots of every SP is dedicated for beacons.
// This module accordingly configures the AP management routines to make sure that all STAs get the necessary beacons when they are awake.
//

#ifndef LMAC_MGMT_IEEE80211LMGMTAP_RTWT_H_
#define LMAC_MGMT_IEEE80211LMGMTAP_RTWT_H_

#include "inet/linklayer/ieee80211/mgmt/Ieee80211MgmtBase.h"
#include "inet/linklayer/ieee80211/mgmt/Ieee80211MgmtAp.h"
#include "src/rtwt/msg/managementInfoFrame_m.h"

namespace inet {
namespace ieee80211 {

class Ieee80211LMgmtApRtwt : public Ieee80211MgmtAp {
    protected:
        virtual void handleAssociationRequestFrame(Packet *packet, const Ptr<const Ieee80211MgmtHeader>& header) override;
        virtual void sendManagementInfo(const MacAddress& staAddr, uint8_t status);
        virtual void sendBeaconTimers(simtime_t startTime, simtime_t beaconInterval);
        virtual void start() override;
};

}
}

#endif /* LMAC_MGMT_IEEE80211LMGMTAP_H_ */
