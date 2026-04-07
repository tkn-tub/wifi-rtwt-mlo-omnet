//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "src/rtwt/msg/twtFrame_m.h"
#include "src/rtwt/msg/twtLinkRequirement_m.h"
#include "src/rtwt/msg/twtScheduleBeacon_m.h"
#include "Ieee80211LLlcLpdRtwt.h"

namespace inet {
namespace ieee80211 {

Define_Module(Ieee80211LLlcLpdRtwt);

void Ieee80211LLlcLpdRtwt::processPacketFromHigherLayer(Packet *packet)
{
    // Handles routing from U-MAC to MAC or TWT agent
    if(strcmp(packet->getName(), "newPackets") == 0) {   // Forward notification about new packets to MAC
        send(packet, "lowerLayerOut");
        return;
    }
    const auto& chunk = packet->peekData();
    if (chunk->getChunkType() == Chunk::CT_FIELDS) {
        if(strcmp(packet->getName(), "twt-requirements") == 0){
            if(auto twtChunk = dynamicPtrCast<const twtLinkRequirement>(chunk)){
                send(packet, "twtAgentOut");
                return;
            }
        } else if (strcmp(packet->getName(), "twt-response") == 0){
            if(auto twtChunk = dynamicPtrCast<const twtFrame>(chunk)){
                send(packet, "twtAgentOut");
                return;
            }
        } else if (strcmp(packet->getName(), "TWT_Beacon") == 0){
            if(auto twtChunk = dynamicPtrCast<const twtScheduleBeacon>(chunk)){
                send(packet, "twtAgentOut");
                return;
            }
        }
    }
    encapsulate(packet);
    send(packet, "lowerLayerOut");
}

void Ieee80211LLlcLpdRtwt::handleMessageWhenUp(cMessage *message){
    if (message->arrivedOn("twtAgentIn")) {
        send(message,"upperLayerOut");
    } else if (message->arrivedOn("mgmtIn")) {
        send(message,"upperLayerOut");
    } else {
        Ieee80211LLlcLpd::handleMessageWhenUp(message);
    }
}

} // namespace ieee80211
} // namespace inet


