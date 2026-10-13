//
//  Created on: April 2025
//      Author: Mateusz Zakrzewski, Doganalp Ergenc (TU Berlin)
//

// U-MAC module for R-TWT enabled STA (generic).
// - It extends the base U-MAC implementation from the MLO project (Umac).
// - STAs announce their link requirements provided in the .ini file.
// - SLO mode is also available.

#ifndef __RTWTUMAC_UMAC_H_
#define __RTWTUMAC_UMAC_H_

#include "src/umac/UMac.h" // MLO

#include <string>
#include <queue>

#include "../lmac/mac/Ieee80211LMacRtwt.h" // NOTE: THIS SHOULD BE ADAPTED - IS IT COMING FROM MLO OR R-TWT MLO?

namespace inet {

class RtwtUMac : public UMac
{
    public:
        std::map<MacAddress,std::deque<Packet*>> pendingByDest;
        virtual void pullPacket(MacAddress awakeMac, int activeSp, int iface);
    protected:
      int sloLink;
      virtual void initialize(int stage) override;
      virtual void handleUpperPacket(Packet *packet) override;
      virtual void handleSelfMessage(cMessage *message) override;
      virtual void handleLowerPacket(Packet *packet) override;
      virtual void parseLinkRequirements();
      virtual void sendTwtPacket(std::string name, int flowId, int restriction, int interfaceId, int type, simtime_t wakeTime, int wakeDuration, int intervalMantissa, int intervalExponent, MacAddress dest);
      virtual void sendTwtLinkRequirements(std::string name, int restriction, int dataRate, MacAddress receiver, uint32_t latency, MacAddress dest, int wakeTime);
    };
}

#endif
