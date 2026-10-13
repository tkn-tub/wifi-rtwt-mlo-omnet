//
//  Created on: April 2025
//      Author: Mateusz Zakrzewski, Doganalp Ergenc (TU Berlin)
//

// The base scheduler implementation. It mainly handles:
// - the collection and processing of TWT link requests
// - calculation of necessary time slots for every request (using max-min fairness).
// - creating TWT responses.
// It is specifically implemented for 2-link MLO. The distribution of the service periods over multiple links is not handled in this base module.

#include <omnetpp.h>

#include <cmath>
#include <algorithm>
#include <string>

#include "src/rtwt/TwtTimeConverter.h"
#include "src/rtwt/msg/twtLinkRequirement_m.h"
#include "src/rtwt/msg/twtFrame_m.h"

#include "TwtSchedulerBase.h"

namespace inet {

Register_Class(TwtSchedulerBase);

int TwtSchedulerBase::numInitStages() const {
    return 13; // enable more Init stages
}

void TwtSchedulerBase::initialize(int stage)
{
    if(stage == INITSTAGE_LINK_LAYER){
        flowIdCounter = 1;
        allRequestsCollected = false;
        practicalBitrate = par("practicalBitrate");
        dataOverhead = par("dataOverhead");
        cModule *parentModule = getParentModule();
        cModule *apModule = parentModule->getParentModule();

        if (apModule->hasSubmodule("umac")) {
            umac = check_and_cast<RtwtUMacAccessPoint*>(apModule->getSubmodule("umac"));
            accessPoint = apModule; //Store AP pointer globaly
        } else {
            EV_ERROR << "Could not find 'umac' module in the AP. R-TWT functionality may not work correctly." << endl;
        }
        
        numInterfaces = accessPoint->par("numWlanInterfaces");
        bitrates = new double[numInterfaces];
        for(int i=0; i<numInterfaces;i++){  //get link bitrates for each interface
            cModule *lmac = accessPoint->getSubmodule("lmac",i);
            bitrates[i] = lmac->par("bitrate");
        }
        minimalInterval = par("minimalInterval");
        scheduleBuffer = par("scheduleBuffer");
        cycleDuration = par("cycleDuration").doubleValue(); // This is in milliseconds
        
        totalSlots = static_cast<int>(std::floor(double(cycleDuration * 1024) / minimalInterval));
        // Initialize array for per-interface slot occupancy
        interfaceSlotsOccupied = new bool*[numInterfaces];
        for (int i = 0; i < numInterfaces; i++) {
            interfaceSlotsOccupied[i] = new bool[totalSlots];
            // Initialize all slots to false (unoccupied)
            for (int j = 0; j < totalSlots; j++) {
                interfaceSlotsOccupied[i][j] = false;
            }
        }
        
        EV_INFO << "TwtScheduler initialized with minimalInterval=" << minimalInterval 
                << "us, scheduleBuffer=" << scheduleBuffer << " (in 256us units), and "
                << "cycleDuration=" << cycleDuration << "ms, with " << totalSlots << " slots" << endl;
    }
}

Ptr<ieee80211::twtFrame> TwtSchedulerBase::createTwtResponseFrame(int flowId, int restriction, int interfaceId, simtime_t wakeTime, int wakeDuration, int intervalMantissa, int intervalExponent, MacAddress STAaddress){
    const auto& response = makeShared<ieee80211::twtFrame>();
    response->setFlowId(flowId);
    response->setType(0); // 0 - response
    response->setRestriction(restriction);
    response->setWakeTime(wakeTime);
    response->setWakeDuration(wakeDuration);
    response->setIntervalMantissa(intervalMantissa);
    response->setIntervalExponent(intervalExponent);
    response->setSTAaddress(STAaddress);
    response->setInterfaceId(interfaceId);
    response->setChunkLength(B(22));
    return response;
}

int TwtSchedulerBase::usToSlots(int us) {
    int slots = static_cast<int>(std::ceil(static_cast<double>(us) / minimalInterval));
    EV_DEBUG << "Converted " << us << "us to " << slots << " slots." << endl;
    return slots;
}

std::pair<uint16_t, uint8_t> TwtSchedulerBase::calculateMantissaExponent(int intervalUs) {
    // Let utility function of TwtTimeConverter handle this calculation
    return TwtTimeConverter::convertMicrosecondsToTwtInterval(intervalUs);
}

bool TwtSchedulerBase::areAllRequestsCollected() {
    // Get all associated hosts from umac interfaceAssociations
    const auto& associations = umac->getInterfaceAssociations();
    std::set<MacAddress> allAssociatedHosts;
    
    // Collect all unique MAC addresses from all interfaces
    for (const auto& [ifaceId, macSet] : associations) {
        for (const auto& macAddr : macSet) {
            allAssociatedHosts.insert(macAddr);
        }
    }
    
    EV_DEBUG << "Total associated hosts: " << allAssociatedHosts.size() << endl;
    
    // Track which hosts have submitted requests (either as sender or receiver)
    std::set<MacAddress> hostsWithRequests;
    for (const auto& req : pendingRequests) {
        hostsWithRequests.insert(req.senderStaAddress);
        hostsWithRequests.insert(req.receiverStaAddress);
    }
    
    EV_DEBUG << "Hosts with requests: " << hostsWithRequests.size() << endl;
    
    // Check if all associated hosts have been involved in at least one request
    for (const auto& host : allAssociatedHosts) {
        if (hostsWithRequests.find(host) == hostsWithRequests.end()) {
            EV_DEBUG << "Host " << host << " has not submitted or received any request yet" << endl;
            return false;
        }
    }
    
    EV_INFO << "All " << allAssociatedHosts.size() << " associated hosts have submitted requests!" << endl;
    return true;
}

void TwtSchedulerBase::handleTwtLinkRequest(Packet *packet) {
    const auto& chunk = packet->peekAtFront<twtLinkRequirement>();
    uint32_t dataRateKbps = chunk->getDataRate();
    uint32_t requestedWakeTimeUs = chunk->getWakeTime();
    MacAddress senderStaAddress = chunk->getSTAaddress();
    MacAddress receiverStaAddress = chunk->getReceiver();

    EV_INFO << "Collecting TWT Link Requirement from STA: " << senderStaAddress
         << " to Receiver: " << receiverStaAddress
         << " DataRate: " << dataRateKbps << " kbps"
         << " RequestedWakeTime: " << requestedWakeTimeUs << " us" << endl;

    // Store the request for later processing
    PendingLinkRequest pendingReq;
    pendingReq.packet = packet->dup(); // Store a copy
    pendingReq.senderStaAddress = senderStaAddress;
    pendingReq.receiverStaAddress = receiverStaAddress;
    pendingReq.dataRateKbps = dataRateKbps;
    pendingReq.requestedWakeTimeUs = requestedWakeTimeUs;
    
    pendingRequests.push_back(pendingReq);
    
    EV_INFO << "Stored request " << pendingRequests.size() << ". Checking if all requests collected" << endl;
    
    // Check if all requests from all associated hosts are collected
    if (areAllRequestsCollected()) {
        EV_INFO << "All requests collected! Starting scheduling" << endl;
        scheduleAllCollectedRequests();
        allRequestsCollected = true;
    }
}

int TwtSchedulerBase::countFreeSlots(int interface){    //Slots are always allocated one after another with no wasted spaces
    if (interface < 0 || interface >= numInterfaces) {
        EV_ERROR << "Invalid interface ID: " << interface << endl;
        return -1;
    }
    int freeSlots = 0;
    for (int i = 0; i < totalSlots; i++) {
        if (!interfaceSlotsOccupied[interface][i]) {
            freeSlots++;
        }
    }
    EV_DEBUG << "Interface " << interface << " has " << totalSlots << " total slots." << endl;
    return freeSlots;
}

TwtSchedulerBase::TwtScheduleResult TwtSchedulerBase::forceSchedule(int startUs, int durationUs, int interfaceId) { //Only for beacons
    TwtScheduleResult result;
    result.success = false;
    if (interfaceId < 0 || interfaceId >= numInterfaces) {
        EV_ERROR << "Invalid interface ID: " << interfaceId << endl;
        return result;
    }

    // Convert startUs to slots
    int startSlot = usToSlots(startUs % (int)(cycleDuration*1024));

    // Convert durationUs to slots
    int durationSlots = usToSlots(durationUs);
    // Find a free slot starting from startSlot
    markSlotsOccupied(startSlot, durationSlots, interfaceId);
    result.success = true;
    
    // For forceSchedule (beacon), we only use one slot, so populate first array elements
    result.senderFlowIds[0] = flowIdCounter++;
    result.senderFlowIds[1] = -1; // Not used for beacon
    result.receiverFlowIds[0] = -1;
    result.receiverFlowIds[1] = -1;
    result.senderInterfaceIds[0] = interfaceId;
    result.senderInterfaceIds[1] = -1; // Not used for beacon
    result.receiverInterfaceIds[0] = interfaceId;
    result.receiverInterfaceIds[1] = -1; // Not used for beacon
    
    result.senderWakeTimes[0] = SimTime(startSlot * minimalInterval, SIMTIME_US);
    while(result.senderWakeTimes[0] < simTime()){
        result.senderWakeTimes[0] += SimTime(cycleDuration*1024, SIMTIME_US);
    }
    result.senderWakeTimes[1] = result.senderWakeTimes[0]; // Same for consistency
    result.receiverWakeTimes[0] = result.senderWakeTimes[0]; // Same for receiver in this case
    result.receiverWakeTimes[1] = result.senderWakeTimes[0]; // Same for consistency
    
    result.wakeDurationSlots = durationSlots;
    auto [intervalMantissa, intervalExponent] = calculateMantissaExponent(cycleDuration*1024);
    result.intervalMantissa = intervalMantissa;
    result.intervalExponent = intervalExponent;
    result.senderStaAddress = MacAddress::BROADCAST_ADDRESS;
    result.receiverStaAddress = MacAddress::BROADCAST_ADDRESS;
    
    // Create a task for this forced schedule
    SchedulerTask task;
    task.start = startSlot;
    task.duration = durationSlots;
    task.interval = usToSlots(cycleDuration*1024);
    task.staAddress = MacAddress::BROADCAST_ADDRESS; // For now, using broadcast address
    task.flowId = result.senderFlowIds[0]; // Using sender flow ID for the task
    task.interfaceId = interfaceId;
    tasks.push_back(task);
    
    EV_INFO << "Forced schedule: Start at " << startUs << "us, Duration " << durationUs 
            << "us, Interval " << cycleDuration*1024 << "us on interface " << interfaceId
            << ". Result: success=" << result.success 
            << ", Sender Flow ID=" << result.senderFlowIds[0] 
            << ", Wake Time=" << result.senderWakeTimes[0] 
            << ", Wake Duration Slots=" << result.wakeDurationSlots 
            << ", Interval Mantissa=" << result.intervalMantissa 
            << ", Interval Exponent=" << (int)result.intervalExponent 
            << endl;
    return result;

}

void TwtSchedulerBase::markSlotsOccupied(int startSlot, int duration, int interfaceId) {    //duration in slots
    if (duration <= 0) {
        EV_ERROR << "Duration must be greater than 0. Provided duration: " << duration << endl;
        recordScalar("scheduler_error_duration_under_1", duration);
        return;
    }
    
    duration = duration + scheduleBuffer; // Add buffer to the duration
    if (interfaceId < 0 || interfaceId >= numInterfaces) {
        EV_ERROR << "Invalid interface ID: " << interfaceId << endl;
        return;
    }
    
    if (startSlot < 0) {
        EV_ERROR << "Invalid startSlot: " << startSlot << endl;
        return;
    }
    
    // Mark slots with circular wrapping
    for (int i = 0; i < duration; i++) {
        int slotToMark = (startSlot + i) % totalSlots;
        interfaceSlotsOccupied[interfaceId][slotToMark] = true;
    }
    
    EV_INFO << "Marked slots occupied starting at " << startSlot
             << " on interface " << interfaceId 
             << " for duration " << duration << " slots (circular marking)" << endl;
             
    // Debug print of updated slot occupancy for this interface
    EV_DEBUG << "Interface " << interfaceId << " updated slot occupancy: ";
    for (int j = 0; j < totalSlots; j++) {
        EV_DEBUG << (interfaceSlotsOccupied[interfaceId][j] ? "1" : "0");
        // Add space every 10 slots for readability
        if ((j + 1) % 10 == 0) {
            EV_DEBUG << " ";
        }
    }
    EV_DEBUG << endl;
}

int TwtSchedulerBase::findFreeSlot(int duration, int startFrom, int interfaceId) { //duration in slots
    duration = duration + scheduleBuffer; // Add buffer to the duration
    if (interfaceId < 0 || interfaceId >= numInterfaces) {
        EV_ERROR << "Invalid interface ID: " << interfaceId << endl;
        return -1;
    }
    
    // Ensure startFrom is within bounds
    startFrom = startFrom % totalSlots;
    
    // Search for a free slot starting from the specified position, wrapping around if needed
    for (int attempt = 0; attempt < totalSlots; attempt++) {
        int startSlot = (startFrom + attempt) % totalSlots;
        bool canFit = true;
        
        // Check if the duration fits (may wrap around the cycle)
        for (int i = 0; i < duration; i++) {
            int slotToCheck = (startSlot + i) % totalSlots;
            if (interfaceSlotsOccupied[interfaceId][slotToCheck]) {
                canFit = false;
                break;
            }
        }

        if (canFit) {
            EV_DEBUG << "Found free slot at " << startSlot << " on interface " << interfaceId 
                     << " for duration " << duration << " slots (circular scheduling)" << endl;
            return startSlot;
        }
    }
    
    EV_DEBUG << "No free slot found on interface " << interfaceId 
             << " for duration " << duration << " slots (checked entire cycle)" << endl;
    return -1;
}

std::vector<double> TwtSchedulerBase::maxMinRequestSlots() {
    EV_INFO << "Scheduling " << pendingRequests.size() << " collected requests optimally..." << endl;
    int cycleUs = static_cast<int>(cycleDuration * 1024); // Convert cycle ms to us

    //calculate average of the requested data rates
    double totalDataRate = 0;
    for (const auto& req : pendingRequests) {
        totalDataRate += req.dataRateKbps * dataOverhead;
    }

    int availableSlots = countFreeSlots(0); // Count free slots on the first interface
    int totalAvailableSlots = availableSlots - (scheduleBuffer * pendingRequests.size()); // Account for buffers

    double freeCyclePart = static_cast<double>(totalAvailableSlots) / totalSlots;

    std::vector<double> requestSlots(pendingRequests.size());
    // Implement min-max fair allocation
    EV_INFO << "Implementing min-max fair allocation for " << pendingRequests.size() << " requests" << endl;

    // Sort requests by data rate (from smallest to largest), saving the index of original request.
    std::vector<std::pair<int, double>> sortedRequests;
    for (int i = 0; i < pendingRequests.size(); i++) {
        sortedRequests.push_back({i, pendingRequests[i].dataRateKbps});
    }
    std::sort(sortedRequests.begin(), sortedRequests.end(), [](const auto& a, const auto& b) { return a.second < b.second; });  //Sort by data rate

    // Initialize all slots to 0
    for (int i = 0; i < pendingRequests.size(); i++) {
        requestSlots[i] = 0;
    }

    double remainingSlots = totalAvailableSlots;
    int remainingFlows = pendingRequests.size();
    double currentAllocation = 0;

    // Allocate slots in max-min fair manner
    for (size_t i = 0; i < sortedRequests.size(); i++) {
        int idx = sortedRequests[i].first;
        double dataRate = sortedRequests[i].second * dataOverhead;
        double requestedSlots = std::ceil(dataRate*1000 / (practicalBitrate*freeCyclePart/2) * totalSlots); //Scale Network capacity to the free part of the cycle. Account for two STAs per request.

        // Calculate fair share with current remaining resources
        double fairShare = remainingSlots / remainingFlows;
        EV_INFO << "Fair share: " << fairShare << endl;
        EV_INFO << "Requested slots: " << requestedSlots << endl;
        EV_INFO << "Requested dataRate: " << dataRate << endl;
        EV_INFO << "Remaining slots: " << remainingSlots << endl;

        // If the flow demand is lower than the fair share, assign the requested slots
        if (i < sortedRequests.size() - 1 && requestedSlots <= fairShare) {
            double allocation = requestedSlots;
            requestSlots[idx] = allocation;
            remainingSlots -= allocation;
            remainingFlows--;
        } else {
            // Allocate fair share to all remaining flows
            for (size_t j = i; j < sortedRequests.size(); j++) {
                requestSlots[sortedRequests[j].first] = std::floor(fairShare);
            }
            break;
        }
    }

    // Round values and ensure totalAvailableSlots is not exceeded
    double totalAssigned = 0;
    for (int i = 0; i < pendingRequests.size(); i++) {
        // Ensure minimum slot allocation (at least 1)
        requestSlots[i] = std::max(1.0, requestSlots[i]);
        totalAssigned += requestSlots[i];
    }

    // Log allocation results
    for (int i = 0; i < pendingRequests.size(); i++) {
        const auto& req = pendingRequests[i];
        EV_INFO << "Request " << i + 1 << ": " << req.senderStaAddress << " -> " << req.receiverStaAddress
                << " allocated " << requestSlots[i] << " slots (" << (requestSlots[i] * 256) << " us)" << endl;
    }

    // Verify if not exceeded allocation
    if (totalAssigned > totalAvailableSlots) {
        EV_ERROR << "Too many assigned slots, assigned: " << totalAssigned << ", available: " << totalAvailableSlots << endl;
        return {};
    }

    return requestSlots;
}

void TwtSchedulerBase::scheduleRequestsWithSlots(const std::vector<double>& requestSlots) { //Function to be extended in scheduler variants
    EV_ERROR << "scheduleRequestsWithSlots not implemented in scheduler base" << endl;
    return;
}

void TwtSchedulerBase::scheduleAllCollectedRequests() {
    std::vector<double> requestSlots = maxMinRequestSlots();
    logRequestSlots(requestSlots);
    scheduleRequestsWithSlots(requestSlots);
}

void TwtSchedulerBase::logRequestSlots(const std::vector<double>& requestSlots){
    int availableSlots = countFreeSlots(0); // Count free slots on the first interface
    int totalAvailableSlots = availableSlots - (scheduleBuffer * pendingRequests.size()); // Account for buffers
    std::string availableStr = "scheduler_slots_availableSlots";
    recordScalar(availableStr.c_str(), availableSlots);
    std::string totalAvailableStr = "scheduler_slots_totalAvailableSlots";
    recordScalar(totalAvailableStr.c_str(), totalAvailableSlots);

    for(int i = 0; i < pendingRequests.size(); i++) {
        const auto& req = pendingRequests[i];
        std::string datarateStr = "scheduler_request_" + req.senderStaAddress.str() + "_dataRate";
        recordScalar(datarateStr.c_str(), req.dataRateKbps);
        std::string slotsStr = "scheduler_request_" + req.senderStaAddress.str() + "_assignedSlots";
        recordScalar(slotsStr.c_str(), requestSlots[i]);
    }
}
void TwtSchedulerBase::logStaSlots(int startSlot, int slots, MacAddress sender, int iface){
    std::string startStr = "scheduler_link-" + std::to_string(iface) + "_" + sender.str() + "_start";
    std::string slotsStr = "scheduler_link-" + std::to_string(iface) + "_" + sender.str() + "_slots";
    recordScalar(startStr.c_str(), startSlot);
    recordScalar(slotsStr.c_str(), slots);
}

}
