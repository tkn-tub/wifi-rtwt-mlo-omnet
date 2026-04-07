//
//  Created on: 01.03.2025
//      Author: zakrzewski
//	TWT Request Link message generated in U-MAC of STA

import inet.common.INETDefs;
import inet.common.packet.chunk.Chunk;
import inet.linklayer.ieee80211.mac.Ieee80211Frame;
import inet.linklayer.common.MacAddress;

namespace inet::ieee80211;


class twtLinkRequirement extends FieldsChunk
{
    uint32_t dataRate;
    uint32_t wakeTime;
    uint8_t restriction;
    uint32_t latency;
    MacAddress receiver;
    MacAddress STAaddress;
}
