//
//  Created on: 13.04.2025
//      Author: zakrzewski
//
#include "../../rtwt/msg/TwtScheduleBeaconSerializer.h"

#include "../../rtwt/msg/twtScheduleBeacon_m.h"
#include "inet/common/packet/serializer/ChunkSerializerRegistry.h"
#include "inet/common/INETDefs.h"

namespace inet {
namespace ieee80211 {

Register_Serializer(twtScheduleBeacon, TwtScheduleBeaconSerializer);

void TwtScheduleBeaconSerializer::serialize(MemoryOutputStream& stream, const Ptr<const Chunk>& chunk) const {
    auto scheduleBeacon = dynamicPtrCast<const twtScheduleBeacon>(chunk);

    uint16_t entryCount = scheduleBeacon->getScheduleEntriesArraySize();
    stream.writeUint16Be(entryCount);

    for (uint16_t i = 0; i < entryCount; i++) {
        const auto& entry = scheduleBeacon->getScheduleEntries(i);

        MacAddress staAddr = entry.getStaAddress();
        for (int i = 0; i < 6; i++) {
            stream.writeUint8(staAddr.getAddressByte(i));
        }

        stream.writeUint64Be(static_cast<uint64_t>(entry.getWakeTime().raw()));
        stream.writeUint32Be(entry.getWakeDuration());
        stream.writeUint16Be(entry.getIntervalMantissa());
        stream.writeUint8(entry.getIntervalExponent());
        stream.writeUint8(entry.getRestriction());
    }
}

const Ptr<Chunk> TwtScheduleBeaconSerializer::deserialize(MemoryInputStream& stream) const {
    auto scheduleBeacon = makeShared<twtScheduleBeacon>();

    // Read entry count
    uint16_t entryCount = stream.readUint16Be();
    scheduleBeacon->setScheduleEntriesArraySize(entryCount);

    for (uint16_t i = 0; i < entryCount; i++) {
        twtScheduleEntry entry;

        uint8_t addrBytes[6];
        for (int i = 0; i < 6; i++) {
            addrBytes[i] = stream.readUint8();
        }
        MacAddress staMac;
        staMac.setAddressBytes(addrBytes);
        entry.setStaAddress(staMac);

        entry.setWakeTime(SimTime::fromRaw(static_cast<int64_t>(stream.readUint64Be())));
        entry.setWakeDuration(stream.readUint32Be());
        entry.setIntervalMantissa(stream.readUint16Be());
        entry.setIntervalExponent(stream.readUint8());
        entry.setRestriction(stream.readUint8());

        scheduleBeacon->setScheduleEntries(i, entry);
    }

    scheduleBeacon->setChunkLength(B(5 + 19 * entryCount));
    return scheduleBeacon;
}

} // namespace ieee80211
} // namespace inet
