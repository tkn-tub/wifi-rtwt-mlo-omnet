//
//  Created on: April 2025
//      Author: Mateusz Zakrzewski, Doganalp Ergenc (TU Berlin)
//

// The base scheduler implementation. It mainly handles:
// - the collection and processing of TWT link requests
// - calculation of necessary time slots for every request (using max-min fairness).
// - creating TWT responses.
// It is specifically implemented for 2-link MLO. The distribution of the service periods over multiple links is not handled in this base module.

#ifndef UMAC_SCHEDULER_TWTSCHEDULERBASE_H_
#define UMAC_SCHEDULER_TWTSCHEDULERBASE_H_
#include <omnetpp.h>
#include "inet/common/packet/Packet.h"

#include "src/umac/RtwtUMacAccessPoint.h"
#include "src/rtwt/msg/twtFrame_m.h"

namespace inet {
using namespace omnetpp;

class TwtSchedulerBase : public cSimpleModule
{
    protected:
        // Scheduler state variables
        int flowIdCounter;
        RtwtUMacAccessPoint *umac;
        cModule *accessPoint;
        int numInterfaces;
        double *bitrates;
        int minimalInterval;
        int scheduleBuffer;
        double cycleDuration;
        int totalSlots;
        bool **interfaceSlotsOccupied;
        double practicalBitrate;
        double dataOverhead;
        // Task structure for tracking scheduled flows
        struct SchedulerTask {
            int start;
            int duration;
            int interval;
            MacAddress staAddress;
            int flowId;
            int interfaceId;
        };
        std::vector<SchedulerTask> tasks;
        
        // Structure for collecting link requests before scheduling
        struct PendingLinkRequest {
            Packet *packet;
            MacAddress senderStaAddress;
            MacAddress receiverStaAddress;
            uint32_t dataRateKbps;
            uint32_t requestedWakeTimeUs;
        };
        std::vector<PendingLinkRequest> pendingRequests;
        bool allRequestsCollected;
        
        // Structure to return scheduling results for multiple interfaces
        struct TwtScheduleResult {
            bool success;
            // Arrays for sender interfaces (4 total: 2 sender + 2 receiver)
            int senderFlowIds[2];        // Flow IDs for sender interface 0 and 1
            int receiverFlowIds[2];      // Flow IDs for receiver interface 0 and 1
            int senderInterfaceIds[2];   // Interface IDs for sender (0 and 1)
            int receiverInterfaceIds[2]; // Interface IDs for receiver (0 and 1) 
            simtime_t senderWakeTimes[2];   // Wake times for sender interfaces
            simtime_t receiverWakeTimes[2]; // Wake times for receiver interfaces
            int wakeDurationSlots;
            uint16_t intervalMantissa;
            uint8_t intervalExponent;
            MacAddress senderStaAddress;
            MacAddress receiverStaAddress;
        };
        virtual int countFreeSlots(int interface);
        
    public:
        virtual int numInitStages() const override;
        virtual void initialize(int stage) override;
        virtual void handleMessage(cMessage *msg) override {}
        
        // TWT scheduling methods
        virtual void handleTwtLinkRequest(Packet *packet);
        virtual void scheduleAllCollectedRequests();
        virtual bool areAllRequestsCollected();
        virtual Ptr<ieee80211::twtFrame> createTwtResponseFrame(int flowId, int restriction, int interfaceId, 
                                                               simtime_t wakeTime, int wakeDuration, 
                                                               int intervalMantissa, int intervalExponent, 
                                                               MacAddress STAaddress);
        
        // Utility methods
        virtual int usToSlots(int us);
        virtual std::pair<uint16_t, uint8_t> calculateMantissaExponent(int intervalUs);
        
        // Slot management methods
        virtual int findFreeSlot(int duration, int startFrom, int interfaceId);
        virtual void markSlotsOccupied(int startSlot, int duration, int interfaceId);
        virtual TwtSchedulerBase::TwtScheduleResult forceSchedule(int startUs, int durationUs, int interfaceId);
        virtual std::vector<double> maxMinRequestSlots();
        virtual void scheduleRequestsWithSlots(const std::vector<double>& requestSlots);
        virtual void logRequestSlots(const std::vector<double>& requestSlots);
        virtual void logStaSlots(int startSlot, int slots, MacAddress sender, int iface);
};

}
#endif /* UMAC_SCHEDULER_TWTSCHEDULERBASE_H_ */
