//
//  Created on: April 2025
//      Author: Mateusz Zakrzewski, Doganalp Ergenc (TU Berlin)
//
//
// This module extends the channel access coordination function HCF in the base MLO framework (which is an extension of 802.11 HCF in INET).
// - It is now capable of talking with the R-TWT agent to perform channel access within the SPs.
// - It pulls packets from U-MAC according to the wake times of the receiver STAs.
// - It employs the R-TWT-aware EDCA module (LEdcaRtwt).
//


#ifndef LMAC_MAC_COORDINATIONFUNCTION_LHCFRTWT_H_
#define LMAC_MAC_COORDINATIONFUNCTION_LHCFRTWT_H_

#include "inet/linklayer/ieee80211/mac/channelaccess/Edca.h"
#include "inet/linklayer/ieee80211/mac/channelaccess/Hcca.h"
#include "inet/linklayer/ieee80211/mac/common/ModeSetListener.h"
#include "inet/linklayer/ieee80211/mac/contract/IAckHandler.h"
#include "inet/linklayer/ieee80211/mac/contract/IBlockAckAgreementHandlerCallback.h"
#include "inet/linklayer/ieee80211/mac/contract/ICoordinationFunction.h"
#include "inet/linklayer/ieee80211/mac/contract/ICtsPolicy.h"
#include "inet/linklayer/ieee80211/mac/contract/IOriginatorBlockAckAgreementHandler.h"
#include "inet/linklayer/ieee80211/mac/contract/IOriginatorBlockAckAgreementPolicy.h"
#include "inet/linklayer/ieee80211/mac/contract/IOriginatorBlockAckProcedure.h"
#include "inet/linklayer/ieee80211/mac/contract/IProcedureCallback.h"
#include "inet/linklayer/ieee80211/mac/contract/IRecipientAckProcedure.h"
#include "inet/linklayer/ieee80211/mac/contract/IRecipientBlockAckAgreementHandler.h"
#include "inet/linklayer/ieee80211/mac/contract/IRecipientBlockAckAgreementPolicy.h"
#include "inet/linklayer/ieee80211/mac/contract/IRecipientBlockAckProcedure.h"
#include "inet/linklayer/ieee80211/mac/contract/IRecipientQosAckPolicy.h"
#include "inet/linklayer/ieee80211/mac/contract/IRecipientQosMacDataService.h"
#include "inet/linklayer/ieee80211/mac/contract/IRtsProcedure.h"
#include "inet/linklayer/ieee80211/mac/contract/ITx.h"
#include "inet/linklayer/ieee80211/mac/framesequence/FrameSequenceContext.h"
#include "inet/linklayer/ieee80211/mac/framesequence/FrameSequenceHandler.h"
#include "inet/linklayer/ieee80211/mac/originator/OriginatorQosMacDataService.h"
#include "inet/linklayer/ieee80211/mac/originator/QosAckHandler.h"
#include "inet/linklayer/ieee80211/mac/originator/QosRecoveryProcedure.h"
#include "inet/linklayer/ieee80211/mac/originator/TxopProcedure.h"
#include "inet/linklayer/ieee80211/mac/protectionmechanism/SingleProtectionMechanism.h"
#include "inet/linklayer/ieee80211/mac/queue/InProgressFrames.h"
#include "inet/linklayer/ieee80211/mac/recipient/CtsProcedure.h"

#include "src/umac/RtwtUMac.h"
#include "src/lmac/mac/channelaccess/LEdcaRtwt.h"
#include "src/rtwt/agent/TwtAgent.h"

namespace inet {

class RtwtUMac; // Forward declaration - might

namespace ieee80211 {

class Ieee80211LMacRtwt;

/**
 * Implements IEEE 802.11 Hybrid Coordination Function.
 */

class LHcfRtwt : public Hcf {

  protected:

    // MODIFICATION BEGIN: New variables for R-TWT
    int udpTid = 0;
    std::map<MacAddress,std::deque<Packet*>> pendingByDest;

    TwtAgent *twtAgent = nullptr;
    RtwtUMac *umac = nullptr; // added for the pull-based approach
    LEdcaRtwt *edca = nullptr;

    bool isTwtMgmtFrame(Packet *packet);
    MacAddress getCurrentSpMac();
    virtual void pullPacketFromUMacQueue();
    // MODIFICATION END

    bool isAckRequired;

    void initialize(int stage) override;
    void handleInternalCollision(std::vector<LEdcafRtwt *> internallyCollidedEdcafs);
    FrameSequenceContext *buildContext(AccessCategory ac);
    void startFrameSequence(AccessCategory ac);
    void transmitFrame(Packet *packet, simtime_t ifs) override;
    void transmissionComplete(Packet *packet, const Ptr<const Ieee80211MacHeader>& header) override;
    bool hasFrameToTransmit() override;
    bool hasFrameToTransmit(AccessCategory ac) override;
    void frameSequenceFinished() override;

    void originatorProcessTransmittedManagementFrame(const Ptr<const Ieee80211MgmtHeader>& mgmtHeader, AccessCategory ac) override;
    void originatorProcessTransmittedControlFrame(const Ptr<const Ieee80211MacHeader>& controlHeader, AccessCategory ac) override;
    void originatorProcessTransmittedDataFrame(Packet *packet, const Ptr<const Ieee80211DataHeader>& dataHeader, AccessCategory ac) override;
    void originatorProcessReceivedManagementFrame(const Ptr<const Ieee80211MgmtHeader>& header, const Ptr<const Ieee80211MacHeader>& lastTransmittedHeader, AccessCategory ac) override;
    void originatorProcessReceivedControlFrame(Packet *packet, const Ptr<const Ieee80211MacHeader>& header, Packet *lastTransmittedPacket, const Ptr<const Ieee80211MacHeader>& lastTransmittedHeader, AccessCategory ac) override;

    void originatorProcessRtsProtectionFailed(Packet *packet) override;
    void originatorProcessTransmittedFrame(Packet *packet) override;
    void originatorProcessReceivedFrame(Packet *packet, Packet *lastTransmittedPacket) override;
    void originatorProcessFailedFrame(Packet *packet) override;

    void sendUp(const std::vector<Packet *>& completeFrames);

  public:
    std::vector<ChannelAccessParameters> getChannelAccessParams();
    void processUpperFrame(Packet *packet, const Ptr<const Ieee80211DataOrMgmtHeader>& header) override;
    void processLowerFrame(Packet *packet, const Ptr<const Ieee80211MacHeader>& header) override;
    virtual void channelGranted(IChannelAccess *channelAccess) override;

    // MODIFICATION BEGIN: Packet pulling functions
    virtual void handleTwtWakeUp();
    virtual void getNewPackets();
    // MODIFICATION END
};

} /* namespace ieee80211 */
} /* namespace inet */

#endif
