//
//  Created on: 13.04.2025
//      Author: zakrzewski
//  Generated at U-MAC, sent to both of the interfaces for transmission

import inet.common.INETDefs;
import inet.common.packet.chunk.Chunk;
import inet.linklayer.ieee80211.mac.Ieee80211Frame;
import inet.linklayer.common.MacAddress;

namespace inet::ieee80211;

class twtScheduleEntry {
    MacAddress staAddress;            
    simtime_t wakeTime;
    uint32_t wakeDuration;
    uint16_t intervalMantissa;
    uint8_t intervalExponent;  
    uint8_t restriction;      
}

class twtScheduleBeacon extends FieldsChunk {
    twtScheduleEntry scheduleEntries[]; // Array of scheduled SPs
}