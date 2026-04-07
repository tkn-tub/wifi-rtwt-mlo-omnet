//
//  Created on: April 2025
//      Author: Mateusz Zakrzewski, Doganalp Ergenc (TU Berlin)
//
//
// The symmetrical scheduler implementation. It divides the calculated service periods equally (into two) and allocates them over two links.
// It resembles with the link aggregation, e.g., the SP is actualized in parallel over two links.
//

#include <omnetpp.h>

#include <cmath>
#include <algorithm>
#include <string>

#include "src/rtwt/msg/twtLinkRequirement_m.h"
#include "src/rtwt/msg/twtFrame_m.h"
#include "src/rtwt/TwtTimeConverter.h"

#include "SymmetricalScheduler.h"

namespace inet {

Register_Class(SymmetricalScheduler);

void SymmetricalScheduler::scheduleRequestsWithSlots(const std::vector<double>& requestSlots) {
    EV_INFO << "Scheduling " << pendingRequests.size() << " requests with predetermined slots..." << endl;
    int cycleUs = static_cast<int>(cycleDuration * 1024); // Convert ms to us
    
    // Collect data for scheudling: interval, slot data
    auto [intervalMantissa, intervalExponent] = calculateMantissaExponent(cycleUs);
    
    int availableSlots = countFreeSlots(0);
    int totalAvailableSlots = availableSlots - (scheduleBuffer * pendingRequests.size());
    EV_INFO << "Total available slots: " << totalAvailableSlots << " slots" << endl;
    
    int currentSlot = 0;

    for (int i = 0; i < pendingRequests.size(); i++) {  //Iterate over all requests
        const auto& req = pendingRequests[i];

        int wakeDurationSlots = (requestSlots[i]-scheduleBuffer)/2;  //Wake duration is halved accounting for buffer. Used later directly as wake duration for SPs.

        
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

        // Find free slots for sender interfaces, avoiding beacon conflicts
        senderStartSlots[0] = findFreeSlot(wakeDurationSlots, currentSlot, senderInterfaces[0]);
        if (senderStartSlots[0] == -1) {
            EV_ERROR << "Could not find free slot for sender interface " << senderInterfaces[0] << endl;
            continue;
        }
        
        senderStartSlots[1] = findFreeSlot(wakeDurationSlots, currentSlot, senderInterfaces[1]);
        if (senderStartSlots[1] == -1) {
            EV_ERROR << "Could not find free slot for sender interface " << senderInterfaces[1] << endl;
            continue;
        }
        
        // Update currentSlot to after the sender allocations
        currentSlot = std::max(senderStartSlots[0], senderStartSlots[1]) + wakeDurationSlots + scheduleBuffer;
        
        // Find free slots for receiver interfaces, avoiding beacon conflicts
        receiverStartSlots[0] = findFreeSlot(wakeDurationSlots, currentSlot, receiverInterfaces[0]);
        if (receiverStartSlots[0] == -1) {
            EV_ERROR << "Could not find free slot for receiver interface " << receiverInterfaces[0] << endl;
            continue;
        }
        
        receiverStartSlots[1] = findFreeSlot(wakeDurationSlots, currentSlot, receiverInterfaces[1]);
        if (receiverStartSlots[1] == -1) {
            EV_ERROR << "Could not find free slot for receiver interface " << receiverInterfaces[1] << endl;
            continue;
        }
        
        // Update currentSlot for next request
        currentSlot = std::max(receiverStartSlots[0], receiverStartSlots[1]) + wakeDurationSlots + scheduleBuffer;
        
        // Mark slots as occupied for all SPs
        markSlotsOccupied(senderStartSlots[0], wakeDurationSlots, senderInterfaces[0]);
        markSlotsOccupied(senderStartSlots[1], wakeDurationSlots, senderInterfaces[1]);
        markSlotsOccupied(receiverStartSlots[0], wakeDurationSlots, receiverInterfaces[0]);
        markSlotsOccupied(receiverStartSlots[1], wakeDurationSlots, receiverInterfaces[1]);
        
        logStaSlots(senderStartSlots[0], wakeDurationSlots, req.senderStaAddress, senderInterfaces[0]);
        logStaSlots(senderStartSlots[1], wakeDurationSlots, req.senderStaAddress, senderInterfaces[1]);
        logStaSlots(receiverStartSlots[0], wakeDurationSlots, req.receiverStaAddress, receiverInterfaces[0]);
        logStaSlots(receiverStartSlots[1], wakeDurationSlots, req.receiverStaAddress, receiverInterfaces[1]);
        
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
        for (int i = 0; i < 2; i++) {   //repeat for every interface
            // Sender interface task
            SchedulerTask senderTask;
            senderTask.start = senderStartSlots[i];
            senderTask.duration = wakeDurationSlots;
            senderTask.interval = usToSlots(cycleUs);
            senderTask.staAddress = req.senderStaAddress;
            senderTask.flowId = senderFlowIds[i];
            senderTask.interfaceId = senderInterfaces[i];
            tasks.push_back(senderTask);
            
            // Receiver interface task
            SchedulerTask receiverTask;
            receiverTask.start = receiverStartSlots[i];
            receiverTask.duration = wakeDurationSlots;
            receiverTask.interval = usToSlots(cycleUs);
            receiverTask.staAddress = req.receiverStaAddress;
            receiverTask.flowId = receiverFlowIds[i];
            receiverTask.interfaceId = receiverInterfaces[i];
            tasks.push_back(receiverTask);
        }
        
        // Create and send TWT response frames for all SPs through TwtUMac
        for (int i = 0; i < 2; i++) {
            umac->scheduleAgreementAndSendResponse(senderInterfaces[i], req.senderStaAddress,senderFlowIds[i], senderWakeTimes[i],wakeDurationSlots, intervalExponent, intervalMantissa);
            umac->scheduleAgreementAndSendResponse(receiverInterfaces[i], req.receiverStaAddress,receiverFlowIds[i], receiverWakeTimes[i],wakeDurationSlots, intervalExponent, intervalMantissa);
        }
        
        EV_INFO << "Scheduled request " << i + 1 << " with optimal utilization:" << endl;
        EV_INFO << "  Sender interfaces " << senderInterfaces[0] << "&" << senderInterfaces[1] 
                << " at slots " << senderStartSlots[0] << "-" << (senderStartSlots[0] + wakeDurationSlots - 1) << endl;
        EV_INFO << "  Receiver interfaces " << receiverInterfaces[0] << "&" << receiverInterfaces[1] 
                << " at slots " << receiverStartSlots[0] << "-" << (receiverStartSlots[0] + wakeDurationSlots - 1) << endl;
    }
    
    // Clean up pending requests
    for (auto& req : pendingRequests) {
        delete req.packet;
    }
    
}

}
