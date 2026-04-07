//
//  Created on: April 2025
//      Author: Mateusz Zakrzewski, Doganalp Ergenc (TU Berlin)
//
// This is fundamentally the same with the LEdca in the base MLO framework (which is an extension of 802.11 Edca in INET). Only the EDCAF module is changed.
// EDCAF module is not flexibly configured - thus we have this mostly duplicate module.
//

#include "LEdcaRtwt.h"

namespace inet {
namespace ieee80211 {

Define_Module(LEdcaRtwt);

void LEdcaRtwt::initialize(int stage)
{
    if (stage == INITSTAGE_LINK_LAYER) {
        numEdcafs = par("numEdcafs");
        edcafs = new LEdcafRtwt *[numEdcafs];
        for (int ac = 0; ac < numEdcafs; ac++) {
            edcafs[ac] = check_and_cast<LEdcafRtwt *>(getSubmodule("edcaf", ac));
        }
        mgmtAndNonQoSRecoveryProcedure = check_and_cast<NonQosRecoveryProcedure *>(getSubmodule("mgmtAndNonQoSRecoveryProcedure"));
    }
}

std::vector<ChannelAccessParameters> LEdcaRtwt::getChannelAccessParams() {
    // Returns immediate backoff counter for each AC on the respective interface
    std::vector<ChannelAccessParameters> parameters;

    for (int ac = 0; ac < numEdcafs; ac++) {
        ChannelAccessParameters p = ChannelAccessParameters{edcafs[ac]->getAccessCategory(), edcafs[ac]->getBackoffCounter()};
        parameters.push_back(p);
    }
    return parameters;
}

void LEdcaRtwt::requestChannelAccess(AccessCategory ac, IChannelAccess::ICallback *callback)
{
    // IMPORTANT: Direct inheritance of this function causes SEGFAULT due to callback functions - thus it is copied.
    edcafs[ac]->requestChannel(callback);
}

void LEdcaRtwt::releaseChannelAccess(AccessCategory ac, IChannelAccess::ICallback *callback)
{
    // IMPORTANT: Direct inheritance of this function causes SEGFAULT due to callback functions - thus it is copied.
    edcafs[ac]->releaseChannel(callback);
}

std::vector<LEdcafRtwt *> LEdcaRtwt::getInternallyCollidedEdcafsModified()
{
    // IMPORTANT: vectors of inherited objects are problematic to cast in virtual functions. Thus this function is modified here and used accordingly within HCF.
    std::vector<LEdcafRtwt *> collidedEdcafs;
    for (int ac = 0; ac < numEdcafs; ac++)
        if (edcafs[ac]->isInternalCollision())
            collidedEdcafs.push_back(edcafs[ac]);
    return collidedEdcafs;
}


LEdcafRtwt *LEdcaRtwt::getChannelOwner()
{
    for (int ac = 0; ac < numEdcafs; ac++)
        if (edcafs[ac]->isOwning())
            return edcafs[ac];
    return nullptr;
}

} // namespace ieee80211
} // namespace inet

