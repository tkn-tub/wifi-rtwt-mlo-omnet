//
//  Created on: April 2025
//      Author: Mateusz Zakrzewski, Doganalp Ergenc (TU Berlin)
//
//
// This is fundamentally the same with the LEdcaf in the base MLO framework (which is an extension of 802.11 Edcaf in INET). Only ACK handling is adapted for SPs.
// ACK handler module is not flexibly configured - thus we have this mostly duplicate module.
//

#ifndef LMAC_MAC_CHANNELACCESS_LEDCAFRTWT_H_
#define LMAC_MAC_CHANNELACCESS_LEDCAFRTWT_H_

#include "inet/linklayer/ieee80211/mac/Ieee80211Frame_m.h"
#include "inet/linklayer/ieee80211/mac/common/AccessCategory.h"
#include "inet/linklayer/ieee80211/mac/common/ModeSetListener.h"
#include "inet/linklayer/ieee80211/mac/channelaccess/Edcaf.h"

#include "src/lmac/mac/contention/LContention.h"
#include "src/lmac/mac/channelaccess/LEdcaf.h"

namespace inet {
namespace ieee80211 {

class LEdcafRtwt : public LEdcaf
{
  protected:
    void initialize(int stage) override;

};

} /* namespace ieee80211 */
} /* namespace inet */

#endif
