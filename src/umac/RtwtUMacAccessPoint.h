//
//  Created on: April 2025
//      Author: Mateusz Zakrzewski, Doganalp Ergenc (TU Berlin)
//

// U-MAC module for R-TWT enabled AP.
// - It extends the base R-TWT enabled U-MAC implementation (RtwtUmac).
// - It has a special queueing facility to orchestrate transmissions within the wake times of the destination STAs.
// - To be able to use R-TWT, it should deploy a scheduler. Symmetrical, asymmetrical, and cross-symmetrical schedulers are available.

#ifndef __RTWTUMAC_UMAC_AP_H_
#define __RTWTUMAC_UMAC_AP_H_

#include "RtwtUMac.h"

namespace inet {

class TwtSchedulerBase;

struct TwtUMacAgreement{
    MacAddress staAddress;      // Use BROADCAST_ADDRESS for broadcast SPs
    uint8_t flowId;
    uint8_t interfaceId;
    simtime_t wakeTime;
    uint32_t wakeDuration;
    uint16_t intervalMantissa;
    uint8_t intervalExponent;
    uint8_t restriction;           // 0 = non-restricted, 1-255 = group ID
    bool isActive;
    cMessage *wakeupMsg;
    cMessage *sleepMsg;
    cMessage *bufferEndMsg;  // Message for end of buffer period
    bool isAwake = false;
    bool inBufferPeriod = false;  // Whether currently in the buffer period
};

class RtwtUMacAccessPoint : public RtwtUMac
{
    protected:
        TwtSchedulerBase *scheduler;
        std::map<int, std::set<MacAddress>> interfaceAssociations;
        std::map<uint8_t, std::map<MacAddress, std::map<uint8_t, TwtUMacAgreement>>> twtAgreementsByInterface;
        virtual void initialize(int stage) override;
        virtual void handleSelfMessage(cMessage *message) override;
        virtual void handleTwtPacket(Packet* packet);
        virtual void handleBeaconTimers(Packet* packet);
        virtual void handleApTwtTimer(cMessage *msg);
        virtual void sendTwtAgreementsBeacon(uint8_t interfaceId);
        virtual void scheduleTwtAgreement(uint8_t interfaceId, const MacAddress& staAddress,
                uint8_t flowId, simtime_t wakeTime,
                uint32_t wakeDuration, uint8_t intervalExponent,
                uint16_t intervalMantissa);
        virtual void handleMgmtInfo(Packet *packet);
        virtual void handleLowerPacket(Packet *packet) override;
    public:
        const std::map<int, std::set<MacAddress>>& getInterfaceAssociations() const {
            return interfaceAssociations;
        }
        std::vector<int> getInterfaceIdsByMacAddress(const MacAddress& mac) const {
            std::vector<int> interfaceIds;
            for (const auto& [ifaceId, macSet] : interfaceAssociations) {
                if (macSet.find(mac) != macSet.end()) {
                    interfaceIds.push_back(ifaceId);
                }
            }
            return interfaceIds;
        }
        virtual void handleTwtLinkRequirements(Packet* packet);
        virtual void sendTwtResponse(Ptr<ieee80211::twtFrame> frame);
        virtual void scheduleAgreementAndSendResponse(uint8_t interfaceId, const MacAddress& staAddress,
                uint8_t flowId, simtime_t wakeTime, uint32_t wakeDuration, 
                uint8_t intervalExponent, uint16_t intervalMantissa);
};
}

#endif /* __RTWTUMAC_UMAC_AP_H_ */
