//
//  Created on: April 2025
//      Author: Mateusz Zakrzewski, Doganalp Ergenc (TU Berlin)
//
//
// This is fundamentally the same with the LEdcaf in the base MLO framework (which is an extension of 802.11 Edcaf in INET). Only ACK handling is adapted for SPs.
// ACK handler module is not flexibly configured - thus we have this mostly duplicate module.
//

#include "inet/linklayer/ieee80211/mac/channelaccess/Edcaf.h"

#include "inet/common/ModuleAccess.h"
#include "inet/common/Simsignals.h"
#include "inet/networklayer/common/NetworkInterface.h"

#include "LEdcafRtwt.h"

namespace inet {
namespace ieee80211 {

Define_Module(LEdcafRtwt);

void LEdcafRtwt::initialize(int stage)
{
    LEdcaf::initialize(stage);
}

} // namespace ieee80211
} // namespace inet

