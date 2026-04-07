//
//  Created on: 01.03.2025
//      Author: zakrzewski
//
#include "../../rtwt/msg/TwtFrameSerializer.h"

#include "../../rtwt/msg/twtFrame_m.h"
#include "inet/common/packet/serializer/ChunkSerializerRegistry.h"
#include "inet/common/INETDefs.h"
namespace inet {
namespace ieee80211 {

Register_Serializer(twtFrame, TwtFrameSerializer);

void TwtFrameSerializer::serialize(MemoryOutputStream& stream, const Ptr<const Chunk>& chunk) const
{
    auto twtFrame = dynamicPtrCast<const class twtFrame>(chunk);

    MacAddress staAddr = twtFrame->getSTAaddress();
    for (int i = 0; i < 6; i++) {
        stream.writeUint8(staAddr.getAddressByte(i));
    }

    stream.writeUint8(twtFrame->getFlowId());
    stream.writeUint8(twtFrame->getType());
    stream.writeUint8(twtFrame->getRestriction());
    stream.writeUint8(twtFrame->getInterfaceId());

    // Serialize simtime_t as raw 64-bit integers (fixed-point)
    stream.writeUint64Be(static_cast<uint64_t>(twtFrame->getWakeTime().raw()));
    stream.writeUint8(twtFrame->getWakeDuration());
    stream.writeUint16Be(twtFrame->getIntervalMantissa());
    stream.writeUint8(twtFrame->getIntervalExponent());

}

const Ptr<Chunk> TwtFrameSerializer::deserialize(MemoryInputStream& stream) const
{
    auto frame = makeShared<twtFrame>();

    uint8_t addrBytes[6];
    for (int i = 0; i < 6; i++) {
        addrBytes[i] = stream.readUint8();
    }
    MacAddress staMac;
    staMac.setAddressBytes(addrBytes);
    frame->setSTAaddress(staMac);

    frame->setFlowId(stream.readUint8());
    frame->setType(stream.readUint8());
    frame->setRestriction(stream.readUint8());
    frame->setInterfaceId(stream.readUint8());

    // Deserialize simtime_t from raw 64-bit integers
    frame->setWakeTime(SimTime::fromRaw(static_cast<int64_t>(stream.readUint64Be())));

    frame->setWakeDuration(stream.readUint8());
    frame->setIntervalMantissa(stream.readUint16Be());
    frame->setIntervalExponent(stream.readUint8());

    // Set correct chunk length
    frame->setChunkLength(B(22));
    return frame;
}


} // namespace ieee80211
} // namespace inet
