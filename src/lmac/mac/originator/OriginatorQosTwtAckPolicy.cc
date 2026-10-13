//
//  Created on: April 2025
//      Author: Mateusz Zakrzewski (TU Berlin)
//
// This module extends the ACK management according to the R-TWT SP management.
// It keeps track of the block ACKs per destination STA, which are supposed to be sent at the end of their specific SPs.
//

#include "inet/linklayer/ieee80211/mac/originator/OriginatorQosAckPolicy.h"
#include "inet/networklayer/common/NetworkInterface.h"
#include "inet/linklayer/ieee80211/mac/Ieee80211Frame_m.h"
#include <tuple>

#include "src/rtwt/agent/TwtAgent.h"
#include "OriginatorQosTwtAckPolicy.h"

namespace inet {
namespace ieee80211 {

Define_Module(OriginatorQosTwtAckPolicy);

// copied from OriginatorQosAckPolicy.cc 
bool OriginatorQosTwtAckPolicy::isBlockAckReqNeeded(InProgressFrames *inProgressFrames, TxopProcedure *txopProcedure) const
{
    // MODIFICATION BEGIN: R-TWT SP awareness.
    TwtAgent *twtAgent = check_and_cast<inet::ieee80211::TwtAgent *>(getContainingNicModule(this)->getSubmodule("twtAgent"));
    MacAddress activeTwtMac = twtAgent->activeTwtSpMacAddr();
    if(activeTwtMac == MacAddress::UNSPECIFIED_ADDRESS){
        activeTwtMac = twtAgent->getLastTwtSpMac();
    }
    // MODIFICATION END

    auto outstandingFramesPerReceiver = getOutstandingFramesPerReceiver(inProgressFrames);
    for (auto outstandingFrames : outstandingFramesPerReceiver) {
        if ((int)outstandingFrames.second.size() >= blockAckReqThreshold){
            if(outstandingFrames.first == activeTwtMac || twtAgent->par("isStation")) // considering MAC address in choosing frames to request block ack
                return true;
        }
    }
    return false;
}

// copied from OriginatorQosAckPolicy.cc 
std::tuple<MacAddress, SequenceNumberCyclic, Tid> OriginatorQosTwtAckPolicy::computeBlockAckReqParameters(InProgressFrames *inProgressFrames, TxopProcedure *txopProcedure) const
{
    // MODIFICATION BEGIN: R-TWT SP awareness.
    TwtAgent *twtAgent = check_and_cast<inet::ieee80211::TwtAgent *>(getContainingNicModule(this)->getSubmodule("twtAgent"));
    MacAddress activeTwtMac = twtAgent->activeTwtSpMacAddr();
    if(activeTwtMac == MacAddress::UNSPECIFIED_ADDRESS){
        activeTwtMac = twtAgent->getLastTwtSpMac();
    }
    // MODIFICATION END
    auto outstandingFramesPerReceiver = getOutstandingFramesPerReceiver(inProgressFrames);
    for (auto outstandingFrames : outstandingFramesPerReceiver) {
        if ((int)outstandingFrames.second.size() >= blockAckReqThreshold) {
            //  MODIFICATION BEGIN: looks for the buffer that matches TWT SP
            if(outstandingFrames.first == activeTwtMac || twtAgent->par("isStation")){
                SequenceNumberCyclic startingSequenceNumber = computeStartingSequenceNumber(outstandingFrames.second);
                Tid tid = outstandingFrames.second.at(0)->peekAtFront<Ieee80211DataHeader>()->getTid();
                return std::make_tuple(outstandingFrames.first, startingSequenceNumber, tid);
            }
            // MODIFICATION END
        }
    }
    return std::make_tuple(MacAddress::UNSPECIFIED_ADDRESS, SequenceNumberCyclic(), -1);
}

}}
