//
//  Created on: 01.04.2025
//      Author: zakrzewski
//	Message sent from AP Mgmt to the U-MAC

import inet.common.INETDefs;
import inet.common.packet.chunk.Chunk;
import inet.linklayer.ieee80211.mac.Ieee80211Frame;
import inet.linklayer.common.MacAddress;

namespace inet::ieee80211;

class ManagementInfoFrame extends FieldsChunk {
    MacAddress staAddress;
    uint8_t interfaceId;
    uint8_t status; // 1 - ASSOCIATED, 0 - DISASSOCIATED
}

