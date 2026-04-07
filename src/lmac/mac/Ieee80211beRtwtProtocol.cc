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

#include "Ieee80211beRtwtProtocol.h"
#include "inet/common/ProtocolGroup.h"

namespace inet {
namespace ieee80211 {

const Protocol *ieee80211beRtwt;

class ProtocolRegistrationRtwt : public cISimulationLifecycleListener
{
  public:
    virtual void lifecycleEvent(SimulationLifecycleEventType eventType, cObject *details) override {
        switch (eventType) {
            case LF_PRE_NETWORK_SETUP:
                ieee80211beRtwt = new Protocol("ieee80211beRtwt", "Wi-Fi 7 R-TWT", Protocol::LinkLayer);
                ProtocolGroup::getEthertypeProtocolGroup()->addProtocol(ieee80211beRtwt->getId(), ieee80211beRtwt);
                break;

            case LF_POST_NETWORK_DELETE:
                ieee80211beRtwt = nullptr;
                break;

            default:
                break;
        }
    }
};

static ProtocolRegistrationRtwt protocolRegistration;
EXECUTE_ON_STARTUP(getEnvir()->addLifecycleListener(&protocolRegistration);)

} // namespace ieee80211
} // namespace inet
