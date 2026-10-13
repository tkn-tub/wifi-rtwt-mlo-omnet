//
//  Created on: April 2025
//      Author: Mateusz Zakrzewski, Doganalp Ergenc (TU Berlin)
//
//
// TWT agent coordinates radio sleep/wake oeprations under the control of U-MAC.
//

#ifndef TWT_TWTAGENT_H_
#define TWT_TWTAGENT_H_
#include <omnetpp.h>
#include "inet/common/IProtocolRegistrationListener.h"
#include "inet/linklayer/ieee80211/mgmt/Ieee80211MgmtBase.h"
#include "inet/linklayer/common/MacAddress.h"
#include "inet/linklayer/ieee80211/mib/Ieee80211Mib.h"

#include "src/rtwt/msg/twtFrame_m.h"

namespace inet {
namespace ieee80211 {

using namespace omnetpp;

struct TwtAgreement {
    simtime_t wakeTime;
    simtime_t wakeDuration;
    simtime_t interval;
    uint8_t flowId;
    uint8_t interfaceId;
    bool isActive;
    cMessage *wakeupMsg;
    cMessage *sleepMsg;
};

class TwtAgent : public cSimpleModule
{
protected:
    MacAddress lastTwtSpMac;
    bool isStation = true;
    int scheduleBuffer = 0;
    virtual int numInitStages() const override;
    virtual void initialize(int stage) override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void handleSelfMessage(cMessage *msg);
    virtual void handleUmacMessage(Packet *packet);
    virtual void forwardToUMac(Packet *packet);
    virtual void handleTwtBeacon(Packet *packet);
    std::map<MacAddress, std::map<uint8_t, TwtAgreement>> twtAgreements;

private:
    MacAddress myMac;
    virtual void processIncomingFrame(Packet *packet);
    virtual void processTwtRequest(Packet* packet);
    virtual void processTwtResponse(Packet* packet);
    virtual void scheduleWakeup(const MacAddress& staAddress, uint8_t flowId, simtime_t wakeTime, simtime_t wakeDuration, simtime_t interval);
    virtual void signalToRadio(uint8_t type, uint32_t offset);
    virtual void signalToLMac(uint8_t type);
public:
    int activeTwtSp();
    MacAddress activeTwtSpMacAddr();
    MacAddress getLastTwtSpMac();
};

}
}
#endif /* TWT_TWTAGENT_H_ */
