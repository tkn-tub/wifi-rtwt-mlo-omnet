//
//  Created on: April 2025
//      Author: Mateusz Zakrzewski, Doganalp Ergenc (TU Berlin)
//
// This is fundamentally the same with the LEdca in the base MLO framework (which is an extension of 802.11 Edca in INET). Only the EDCAF module is changed.
// EDCAF module is not flexibly configured - thus we have this mostly duplicate module.
//

#ifndef LMAC_MAC_CHANNELACCESS_LEDCARTWT_H_
#define LMAC_MAC_CHANNELACCESS_LEDCARTWT_H_

#include "inet/linklayer/ieee80211/mac/channelaccess/Edca.h"
#include "LEdcafRtwt.h"

#include "inet/linklayer/ieee80211/mac/common/Ieee80211Defs.h"
#include "inet/linklayer/ieee80211/mac/lifetime/EdcaTransmitLifetimeHandler.h"
#include "inet/linklayer/ieee80211/mac/originator/NonQosRecoveryProcedure.h"

#include "src/lmac/mac/channelaccess/LEdca.h"

namespace inet {
namespace ieee80211 {

class LEdcaRtwt : public Edca
{
    public:
        std::vector<ChannelAccessParameters> getChannelAccessParams();
        LEdcafRtwt *getEdcaf(AccessCategory ac) const override { return edcafs[ac]; }
        LEdcafRtwt *getChannelOwner() override;
        virtual void requestChannelAccess(AccessCategory ac, IChannelAccess::ICallback *callback) override;
        virtual void releaseChannelAccess(AccessCategory ac, IChannelAccess::ICallback *callback) override;
        std::vector<LEdcafRtwt *> getInternallyCollidedEdcafsModified();

    protected:
        LEdcafRtwt **edcafs = nullptr;
        void initialize(int stage) override;

};

} /* namespace ieee80211 */
} /* namespace inet */

#endif

