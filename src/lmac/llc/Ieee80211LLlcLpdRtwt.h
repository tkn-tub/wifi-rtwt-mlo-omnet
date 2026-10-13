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

#ifndef LMAC_LLC_IEEE80211LLLCLPDRTWT_H_
#define LMAC_LLC_IEEE80211LLLCLPDRTWT_H_

#include "src/lmac/llc/Ieee80211LLlcLpd.h"

namespace inet {
namespace ieee80211 {

class Ieee80211LLlcLpdRtwt : public Ieee80211LLlcLpd
{
      protected:
        virtual void processPacketFromHigherLayer(Packet *packet) override;
        virtual void handleMessageWhenUp(cMessage *message) override;
};

} // namespace ieee80211
} // namespace inet

#endif
