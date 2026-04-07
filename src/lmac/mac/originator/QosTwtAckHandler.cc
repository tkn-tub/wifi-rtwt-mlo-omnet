//
//  Created on: April 2025
//      Author: Mateusz Zakrzewski (TU Berlin)
//
// This module extends the ACK management according to the R-TWT SP management.
// It keeps track of the block ACKs per destination STA, which are supposed to be sent at the end of their specific SPs.
//

#include "inet/linklayer/ieee80211/mac/originator/QosAckHandler.h"
#include "QosTwtAckHandler.h"

namespace inet {
namespace ieee80211 {

Define_Module(QosTwtAckHandler);

// copied from INET QosAckHandler.cc and adjusted for selecting correct MAC address
void QosTwtAckHandler::processTransmittedBlockAckReq(const Ptr<const Ieee80211BlockAckReq>& blockAckReq)
{
    for (auto& ackStatus : ackStatuses) {
        auto tid = ackStatus.first.second.first;
        auto seqCtrlField = ackStatus.first.second.second;
        auto& status = ackStatus.second;
        auto macAddr = ackStatus.first.first;   // get MAC address from ackStatuses
        if (auto basicBlockAckReq = dynamicPtrCast<const Ieee80211BasicBlockAckReq>(blockAckReq)) {
            if (basicBlockAckReq->getTidInfo() == tid && blockAckReq->getReceiverAddress() == macAddr) { // check if MAC address matches
                auto startingSeqNum = basicBlockAckReq->getStartingSequenceNumber();
                if (status == Status::BLOCK_ACK_NOT_YET_REQUESTED && SequenceNumberCyclic(seqCtrlField.getSequenceNumber()) >= startingSeqNum)
                    status = Status::WAITING_FOR_BLOCK_ACK;
            }
        }
        else if (auto compressedBlockAckReq = dynamicPtrCast<const Ieee80211CompressedBlockAckReq>(blockAckReq)) {
            if (compressedBlockAckReq->getTidInfo() == tid) {
                auto startingSeqNum = compressedBlockAckReq->getStartingSequenceNumber();
                if (status == Status::BLOCK_ACK_NOT_YET_REQUESTED && SequenceNumberCyclic(seqCtrlField.getSequenceNumber()) >= startingSeqNum && seqCtrlField.getFragmentNumber() == 0) // TODO ASSERT(seqCtrlField.second == 0)?
                    status = Status::WAITING_FOR_BLOCK_ACK;
            }
        }
        else
            throw cRuntimeError("Multi-TID BlockReq is unimplemented");
    }
}

}}
