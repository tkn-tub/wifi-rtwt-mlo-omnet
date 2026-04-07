//
//  Created on: April 2025
//      Author: Mateusz Zakrzewski (TU Berlin)
//
// This module extends the L-MAC interfaces in the base MLO framework (Ieee80211LMac).
// It includes a new R-TWT agent, and uses the extended R-TWT-aware HCF.
//

#ifndef __IEEE80211LMACRTWT_H
#define __IEEE80211LMACRTWT_H

#include "Ieee80211LMacRtwt.h"

#include "inet/linklayer/ieee80211/mac/Ieee80211Mac.h"
#include "inet/common/ModuleRefByPar.h"
#include "inet/linklayer/ieee80211/mac/contract/IDs.h"
#include "inet/linklayer/ieee80211/mac/contract/IRateControl.h"
#include "inet/linklayer/ieee80211/mac/contract/IRateSelection.h"
#include "inet/linklayer/ieee80211/mac/contract/IRx.h"
#include "inet/linklayer/ieee80211/mac/contract/ITx.h"
#include "inet/linklayer/ieee80211/mac/coordinationfunction/Dcf.h"
#include "inet/linklayer/ieee80211/mac/coordinationfunction/Hcf.h"
#include "inet/linklayer/ieee80211/mac/coordinationfunction/Mcf.h"
#include "inet/linklayer/ieee80211/mac/coordinationfunction/Pcf.h"
#include "inet/linklayer/ieee80211/mib/Ieee80211Mib.h"
#include "inet/physicallayer/wireless/common/contract/packetlevel/IRadio.h"

#include "coordinationfunction/LHcfRtwt.h"
#include "src/rtwt/agent/TwtAgent.h"

namespace inet {
namespace ieee80211 {

class IContention;
class IRx;
class IIeee80211Llc;
class Ieee80211MacHeader;
class LHcfRtwt;

class Ieee80211LMacRtwt : public Ieee80211LMac {

   protected:

       // MODIFICATION BEGIN: R-TWT variables and updated functions. New HCF definition is especially important.
       LHcfRtwt *hcf = nullptr;
       TwtAgent *twtAgent = nullptr;
       bool twtEnable = false;
       virtual void handleMessageWhenUp(cMessage *message) override;
       virtual void handleTwtPacket(Packet *packet);
       virtual void handleTwtWakeUp();
       // MODIFICATION END
       virtual void initialize(int) override;

       /** @brief Handle messages from lower (physical) layer */
       virtual void handleLowerPacket(Packet *packet) override;

       virtual void encapsulate(Packet *packet) override;
       virtual void decapsulate(Packet *packet) override;

   public:
       Ieee80211LMacRtwt();
       virtual ~Ieee80211LMacRtwt();

       virtual void sendUp(cMessage *message) override;
       virtual void sendUpFrame(Packet *frame) override;

       virtual void processUpperFrame(Packet *packet, const Ptr<const Ieee80211DataOrMgmtHeader>& header) override;
       virtual void processLowerFrame(Packet *packet, const Ptr<const Ieee80211MacHeader>& header) override;

       std::vector<ChannelAccessParameters> getChannelAccessParams();
};

} // namespace ieee80211
} // namespace inet

#endif
