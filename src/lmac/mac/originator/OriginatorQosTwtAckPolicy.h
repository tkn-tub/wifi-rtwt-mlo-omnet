//
//  Created on: April 2025
//      Author: Mateusz Zakrzewski (TU Berlin)
//
// This module extends the ACK management according to the R-TWT SP management.
// It keeps track of the block ACKs per destination STA, which are supposed to be sent at the end of their specific SPs.
//

#ifndef LMAC_MAC_ORIGINATOR_ORIGINATORQOSTWTACKPOLICY_H_
#define LMAC_MAC_ORIGINATOR_ORIGINATORQOSTWTACKPOLICY_H_

#include "inet/linklayer/ieee80211/mac/originator/OriginatorQosAckPolicy.h"

namespace inet {
namespace ieee80211 {

class OriginatorQosTwtAckPolicy : public OriginatorQosAckPolicy
{
  public:
    virtual bool isBlockAckReqNeeded(InProgressFrames *inProgressFrames, TxopProcedure *txopProcedure) const override;
    virtual std::tuple<MacAddress, SequenceNumberCyclic, Tid> computeBlockAckReqParameters(InProgressFrames *inProgressFrames, TxopProcedure *txopProcedure) const override;
};

} /* namespace ieee80211 */
} /* namespace inet */


#endif /* LMAC_MAC_ORIGINATOR_ORIGINATORQOSTWTACKPOLICY_H_ */
