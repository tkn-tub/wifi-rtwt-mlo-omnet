//
//  Created on: April 2025
//      Author: Mateusz Zakrzewski, Doganalp Ergenc (TU Berlin)
//

// The asymmetrical scheduler implementation. It divides the calculated service periods proportional to the data rates at each link.
// Differently than the symmetrical scheduler, it also enables simultaneous transmission and reception.

#include <omnetpp.h>
#include <cmath>
#include <algorithm>
#include <string>

#include "src/rtwt/scheduler/AsymmetricalScheduler.h"
#include "src/rtwt/TwtTimeConverter.h"
#include "src/rtwt/msg/twtLinkRequirement_m.h"
#include "src/rtwt/msg/twtFrame_m.h"

namespace inet {

Register_Class(AsymmetricalScheduler);

int AsymmetricalScheduler::calculatePurgeSlots(int wakeDurationSlots) { //Calculate slots for purging packets from AP queue
    double senderRate = static_cast<double>(bitrates[1]);
    double receiverRate = static_cast<double>(bitrates[0]);
    double senderPerSlot = senderRate * 0.000256;
    double receiverPerSlot = receiverRate * 0.000256;
    double senderDurationSlots = ((senderPerSlot + receiverPerSlot)*wakeDurationSlots - senderPerSlot * scheduleBuffer)/(2*senderPerSlot);
    int purgeSlots = wakeDurationSlots - static_cast<int>(senderDurationSlots) - scheduleBuffer;
    return purgeSlots;
}

void AsymmetricalScheduler::scheduleRequestsWithSlots(const std::vector<double>& requestSlots) {
    EV_INFO << "Scheduling " << pendingRequests.size() << " requests with predetermined slots..." << endl;
    int cycleUs = static_cast<int>(cycleDuration * 1024); // Convert ms to us
    
    // Collect data for scheudling: interval, slot data
    auto [intervalMantissa, intervalExponent] = calculateMantissaExponent(cycleUs);
    

    int availableSlots = countFreeSlots(0);
    int totalAvailableSlots = availableSlots - (scheduleBuffer * pendingRequests.size());
    EV_INFO << "Total available slots: " << totalAvailableSlots << " slots" << endl;
    
    int currentSlot = 0;

    for (int i = 0; i < pendingRequests.size(); i++) { //Iterate over all requests
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

        // Schedule 4 service periods for this request, avoiding beacon slots
        int senderStartSlots[2];
        int receiverStartSlots[2];
        
        // Calculate Wake Durations for both sender and receiver
        // Calculate how many slots are needed to purge all queued packets in the AP. 

        int purgeDurationSlots = calculatePurgeSlots(wakeDurationSlots);

        senderStartSlots[0] = -1;
        senderStartSlots[1] = findFreeSlot(wakeDurationSlots-purgeDurationSlots-scheduleBuffer, currentSlot, senderInterfaces[1]);
        if (senderStartSlots[1] == -1) {
            EV_ERROR << "Could not find free slot for sender interface " << senderInterfaces[1] << endl;
            continue;
        }
        
        // Find free slots for receiver interfaces, avoiding beacon conflicts
        receiverStartSlots[0] = findFreeSlot(wakeDurationSlots, currentSlot, receiverInterfaces[0]);
        if (receiverStartSlots[0] == -1) {
            EV_ERROR << "Could not find free slot for receiver interface " << receiverInterfaces[0] << endl;
            continue;
        }
        
        receiverStartSlots[1] = findFreeSlot(purgeDurationSlots, senderStartSlots[1]+wakeDurationSlots-purgeDurationSlots, receiverInterfaces[1]);
        if (receiverStartSlots[1] == -1) {
            EV_ERROR << "Could not find free slot for receiver interface " << receiverInterfaces[1] << endl;
            continue;
        }
        // Mark slots as occupied for all SPs
        markSlotsOccupied(senderStartSlots[1], wakeDurationSlots-purgeDurationSlots-scheduleBuffer, senderInterfaces[1]);
        markSlotsOccupied(receiverStartSlots[0], wakeDurationSlots, receiverInterfaces[0]);
        markSlotsOccupied(receiverStartSlots[1], purgeDurationSlots, receiverInterfaces[1]);
    
        logStaSlots(senderStartSlots[1], wakeDurationSlots-purgeDurationSlots-scheduleBuffer, req.senderStaAddress, senderInterfaces[1]);
        logStaSlots(receiverStartSlots[0], wakeDurationSlots, req.receiverStaAddress, receiverInterfaces[0]);
        logStaSlots(receiverStartSlots[1], purgeDurationSlots, req.receiverStaAddress, receiverInterfaces[1]);
        

        // Convert slot times to absolute wake times considering requested wake time
        simtime_t senderWakeTimes[2];
        simtime_t receiverWakeTimes[2];
        
        for (int i = 0; i < 2; i++) {
            senderWakeTimes[i] = SimTime(senderStartSlots[i] * minimalInterval, SIMTIME_US);
            receiverWakeTimes[i] = SimTime(receiverStartSlots[i] * minimalInterval, SIMTIME_US);
            //std::cout << "senderWakeTime " << i << ": " << senderWakeTimes[i] << endl;
            //std::cout << "receiverWakeTimes " << i << ": " << receiverWakeTimes[i] << endl;
            while(senderWakeTimes[i] < SimTime(req.requestedWakeTimeUs, SIMTIME_US)){
                senderWakeTimes[i] += SimTime(cycleUs, SIMTIME_US);
            }
            while(receiverWakeTimes[i] < SimTime(req.requestedWakeTimeUs, SIMTIME_US)){
                receiverWakeTimes[i] += SimTime(cycleUs, SIMTIME_US);
            }
        }
        
        // Create flow IDs for all SPs
        int senderFlowIds[2] = {-1, flowIdCounter++};
        int receiverFlowIds[2] = {flowIdCounter++, flowIdCounter++};
        
        // Store scheduler tasks
        // Sender interface task
        SchedulerTask senderTask;
        senderTask.start = senderStartSlots[1];
        senderTask.duration = wakeDurationSlots-purgeDurationSlots-scheduleBuffer;
        senderTask.interval = usToSlots(cycleUs);
        senderTask.staAddress = req.senderStaAddress;
        senderTask.flowId = senderFlowIds[1];
        senderTask.interfaceId = senderInterfaces[1];
        tasks.push_back(senderTask);

        // Receiver interface task
        SchedulerTask receiverTask0;
        receiverTask0.start = receiverStartSlots[0];
        receiverTask0.duration = wakeDurationSlots;
        receiverTask0.interval = usToSlots(cycleUs);
        receiverTask0.staAddress = req.receiverStaAddress;
        receiverTask0.flowId = receiverFlowIds[0];
        receiverTask0.interfaceId = receiverInterfaces[0];
        tasks.push_back(receiverTask0);

        // Receiver interface task
        SchedulerTask receiverTask1;
        receiverTask1.start = receiverStartSlots[1];
        receiverTask1.duration = purgeDurationSlots;
        receiverTask1.interval = usToSlots(cycleUs);
        receiverTask1.staAddress = req.receiverStaAddress;
        receiverTask1.flowId = receiverFlowIds[1];
        receiverTask1.interfaceId = receiverInterfaces[1];
        tasks.push_back(receiverTask1);

        // Create and send TWT response frames for all SPs through TwtUMac
        umac->scheduleAgreementAndSendResponse(senderInterfaces[1], req.senderStaAddress,senderFlowIds[1], senderWakeTimes[1],wakeDurationSlots-purgeDurationSlots-scheduleBuffer, intervalExponent, intervalMantissa);
        umac->scheduleAgreementAndSendResponse(receiverInterfaces[0], req.receiverStaAddress,receiverFlowIds[0], receiverWakeTimes[0],wakeDurationSlots, intervalExponent, intervalMantissa);
        umac->scheduleAgreementAndSendResponse(receiverInterfaces[1], req.receiverStaAddress,receiverFlowIds[1], receiverWakeTimes[1],purgeDurationSlots, intervalExponent, intervalMantissa);
    }
    
    // Clean up pending requests
    for (auto& req : pendingRequests) {
        delete req.packet;
    }
}

}
