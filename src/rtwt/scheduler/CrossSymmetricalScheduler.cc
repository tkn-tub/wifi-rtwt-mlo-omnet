//
//  Created on: April 2025
//      Author: Mateusz Zakrzewski, Doganalp Ergenc (TU Berlin)
//

// The cross-symmetrical scheduler implementation. It divides the calculated service periods equally (similar to the Symmetrical scheduler).
// However, instead of aggregating links, it enables simultaneous tranmission and reception (i.e., sender SP at one link and receiver SP at another link).
// Note that this simultaneous tranmission and reception does NOT mean the STR in MLO context. Here, the tranmission and reception occur at the sender and the receiver MLDs respectively.
// Nevertheless, all MLO operations in our implementation is STR-enabled.

#include <omnetpp.h>
#include <cmath>
#include <algorithm>
#include <string>

#include "src/rtwt/TwtTimeConverter.h"
#include "src/rtwt/msg/twtLinkRequirement_m.h"
#include "src/rtwt/msg/twtFrame_m.h"
#include "CrossSymmetricalScheduler.h"

namespace inet {

Register_Class(CrossSymmetricalScheduler);

void CrossSymmetricalScheduler::scheduleRequestsWithSlots(const std::vector<double>& requestSlots) {
    EV_INFO << "Scheduling " << pendingRequests.size() << " requests with predetermined slots..." << endl;
    int cycleUs = static_cast<int>(cycleDuration * 1024); // Convert ms to us
    
    auto [intervalMantissa, intervalExponent] = calculateMantissaExponent(cycleUs);
    
    // Collect data for scheduling: interval, slot data
    int availableSlots = countFreeSlots(0);
    int totalAvailableSlots = availableSlots - (scheduleBuffer * pendingRequests.size());
    EV_INFO << "Total available slots: " << totalAvailableSlots << " slots" << endl;
    
    int currentSlot = 0;

    for (int i = 0; i < pendingRequests.size(); i++) {  //Iterate over all requests
        const auto& req = pendingRequests[i];

        int wakeDurationSlots = requestSlots[i];

        
        EV_INFO << "Request " << i + 1 << ": " << req.senderStaAddress << " -> " << req.receiverStaAddress
                << " allocated " << wakeDurationSlots << " slots (" << (wakeDurationSlots * 256) << " us)" << endl;
        
        // Get interfaces for sender and receiver
        auto ifaceIdsSender = umac->getInterfaceIdsByMacAddress(req.senderStaAddress);
        auto ifaceIdsReceiver = umac->getInterfaceIdsByMacAddress(req.receiverStaAddress);
        
        // Get interface IDs (assuming each station has interfaces 0 and 1)
        int senderInterfaces[2] = {ifaceIdsSender[0], ifaceIdsSender[1]};
        int receiverInterfaces[2] = {ifaceIdsReceiver[0], ifaceIdsReceiver[1]};

        int senderStartSlots[2];
        int receiverStartSlots[2];
        
        int wakeSlots = (wakeDurationSlots - scheduleBuffer)/2; //calculate wake slots for each SP (symmetric pair)

        // Find free slots for receiver interfaces, avoiding beacon conflicts
        senderStartSlots[1] = findFreeSlot(wakeSlots, currentSlot, senderInterfaces[1]);
        if (senderStartSlots[1] == -1) {
            EV_ERROR << "Could not find free slot for sender interface " << senderInterfaces[1] << endl;
            continue;
        }
        
        receiverStartSlots[0] = findFreeSlot(wakeSlots, currentSlot, receiverInterfaces[0]);
        if (receiverStartSlots[0] == -1) {
            EV_ERROR << "Could not find free slot for receiver interface " << receiverInterfaces[0] << endl;
            continue;
        }
        
        senderStartSlots[0] = findFreeSlot(wakeSlots, receiverStartSlots[0]+scheduleBuffer+wakeSlots, senderInterfaces[1]);
        if (senderStartSlots[1] == -1) {
            EV_ERROR << "Could not find free slot for sender interface " << senderInterfaces[1] << endl;
            continue;
        }

        receiverStartSlots[1] = findFreeSlot(wakeSlots, receiverStartSlots[0]+scheduleBuffer+wakeSlots, receiverInterfaces[1]);
        if (receiverStartSlots[1] == -1) {
            EV_ERROR << "Could not find free slot for receiver interface " << receiverInterfaces[1] << endl;
            continue;
        }

        // Mark slots as occupied for all SPs
        markSlotsOccupied(senderStartSlots[0], wakeSlots, senderInterfaces[0]);
        markSlotsOccupied(senderStartSlots[1], wakeSlots, senderInterfaces[1]);
        markSlotsOccupied(receiverStartSlots[0], wakeSlots, receiverInterfaces[0]);
        markSlotsOccupied(receiverStartSlots[1], wakeSlots, receiverInterfaces[1]);
        logStaSlots(senderStartSlots[0], wakeSlots, req.senderStaAddress, senderInterfaces[0]);
        logStaSlots(senderStartSlots[1], wakeSlots, req.senderStaAddress, senderInterfaces[1]);
        logStaSlots(receiverStartSlots[0], wakeSlots, req.receiverStaAddress, receiverInterfaces[0]);
        logStaSlots(receiverStartSlots[1], wakeSlots, req.receiverStaAddress, receiverInterfaces[1]);
        
        // Convert slot times to absolute wake times considering requested wake time
        simtime_t senderWakeTimes[2];
        simtime_t receiverWakeTimes[2];
        
        for (int i = 0; i < 2; i++) {
            senderWakeTimes[i] = SimTime(senderStartSlots[i] * minimalInterval, SIMTIME_US);
            receiverWakeTimes[i] = SimTime(receiverStartSlots[i] * minimalInterval, SIMTIME_US);
            
            while(senderWakeTimes[i] < SimTime(req.requestedWakeTimeUs, SIMTIME_US)){
                senderWakeTimes[i] += SimTime(cycleUs, SIMTIME_US);
            }
            while(receiverWakeTimes[i] < SimTime(req.requestedWakeTimeUs, SIMTIME_US)){
                receiverWakeTimes[i] += SimTime(cycleUs, SIMTIME_US);
            }
        }
        
        // Create flow IDs for all SPs
        int senderFlowIds[2] = {flowIdCounter++, flowIdCounter++};
        int receiverFlowIds[2] = {flowIdCounter++, flowIdCounter++};
        
        // Store scheduler tasks
        // Sender interface0 task
        SchedulerTask senderTask0;
        senderTask0.start = senderStartSlots[0];
        senderTask0.duration = wakeSlots;
        senderTask0.interval = usToSlots(cycleUs);
        senderTask0.staAddress = req.senderStaAddress;
        senderTask0.flowId = senderFlowIds[0];
        senderTask0.interfaceId = senderInterfaces[0];
        tasks.push_back(senderTask0);

        // Sender interface1 task
        SchedulerTask senderTask1;
        senderTask1.start = senderStartSlots[1];
        senderTask1.duration = wakeSlots;
        senderTask1.interval = usToSlots(cycleUs);
        senderTask1.staAddress = req.senderStaAddress;
        senderTask1.flowId = senderFlowIds[1];
        senderTask1.interfaceId = senderInterfaces[1];
        tasks.push_back(senderTask1);

        // Receiver interface0 task
        SchedulerTask receiverTask0;
        receiverTask0.start = receiverStartSlots[0];
        receiverTask0.duration = wakeSlots;
        receiverTask0.interval = usToSlots(cycleUs);
        receiverTask0.staAddress = req.receiverStaAddress;
        receiverTask0.flowId = receiverFlowIds[0];
        receiverTask0.interfaceId = receiverInterfaces[0];
        tasks.push_back(receiverTask0);

        // Receiver interface1 task
        SchedulerTask receiverTask1;
        receiverTask1.start = receiverStartSlots[1];
        receiverTask1.duration = wakeSlots;
        receiverTask1.interval = usToSlots(cycleUs);
        receiverTask1.staAddress = req.receiverStaAddress;
        receiverTask1.flowId = receiverFlowIds[1];
        receiverTask1.interfaceId = receiverInterfaces[1];
        tasks.push_back(receiverTask1);

        // Create and send TWT response frames for all SPs through TwtUMac
        umac->scheduleAgreementAndSendResponse(senderInterfaces[0], req.senderStaAddress,senderFlowIds[0], senderWakeTimes[0],wakeSlots, intervalExponent, intervalMantissa);
        umac->scheduleAgreementAndSendResponse(senderInterfaces[1], req.senderStaAddress,senderFlowIds[1], senderWakeTimes[1],wakeSlots, intervalExponent, intervalMantissa);
        umac->scheduleAgreementAndSendResponse(receiverInterfaces[0], req.receiverStaAddress,receiverFlowIds[0], receiverWakeTimes[0],wakeSlots, intervalExponent, intervalMantissa);
        umac->scheduleAgreementAndSendResponse(receiverInterfaces[1], req.receiverStaAddress,receiverFlowIds[1], receiverWakeTimes[1],wakeSlots, intervalExponent, intervalMantissa);
    }
    
    // Clean up pending requests
    for (auto& req : pendingRequests) {
        delete req.packet;
    }
}

}
