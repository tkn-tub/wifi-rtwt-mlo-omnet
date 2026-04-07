//
//  Created on: 25.06.2025
//      Author: zakrzewski
// Message sent from TWT Agent to the TWT Radio

import inet.common.INETDefs;
import inet.common.packet.chunk.Chunk;
import inet.linklayer.ieee80211.mac.Ieee80211Frame;
import inet.linklayer.common.MacAddress;

namespace inet::ieee80211;


class signalRadio extends FieldsChunk
{
    uint8_t type;
    uint32_t offset;
}

