//
//  Created on: April 2025
//      Author: Mateusz Zakrzewski (TU Berlin)
//
// This module extends the ACK management according to the R-TWT SP management.
// It keeps track of the block ACKs per destination STA, which are supposed to be sent at the end of their specific SPs.
//

#ifndef LMAC_MAC_ORIGINATOR_QOSNOACKHANDLER_H_
#define LMAC_MAC_ORIGINATOR_QOSNOACKHANDLER_H_

#include "inet/linklayer/ieee80211/mac/originator/QosAckHandler.h"

namespace inet {
namespace ieee80211 {

class QosTwtAckHandler : public QosAckHandler
{
  public:
    virtual void processTransmittedBlockAckReq(const Ptr<const Ieee80211BlockAckReq>& blockAckReq) override;
};

} /* namespace ieee80211 */
} /* namespace inet */

#endif

