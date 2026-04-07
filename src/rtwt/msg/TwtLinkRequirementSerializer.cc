//
//  Created on: 01.03.2025
//      Author: zakrzewski
//

#include "../../rtwt/msg/TwtLinkRequirementSerializer.h"

#include "../../rtwt/msg/twtLinkRequirement_m.h"
#include "inet/common/packet/serializer/ChunkSerializerRegistry.h"
#include "inet/common/INETDefs.h"
namespace inet {
namespace ieee80211 {

Register_Serializer(twtLinkRequirement, TwtLinkRequirementSerializer);

void TwtLinkRequirementSerializer::serialize(MemoryOutputStream& stream, const Ptr<const Chunk>& chunk) const
{
    auto linkReq = dynamicPtrCast<const class twtLinkRequirement>(chunk);

    MacAddress staAddr = linkReq->getSTAaddress();
    for (int i = 0; i < 6; i++) {
        stream.writeUint8(staAddr.getAddressByte(i));
    }
    MacAddress receiver = linkReq->getReceiver();
    for (int i = 0; i < 6; i++) {
        stream.writeUint8(receiver.getAddressByte(i));
    }

    stream.writeUint32Be(linkReq->getDataRate());
    stream.writeUint32Be(linkReq->getWakeTime());
    stream.writeUint8(linkReq->getRestriction());
    stream.writeUint32Be(linkReq->getLatency());
}

const Ptr<Chunk> TwtLinkRequirementSerializer::deserialize(MemoryInputStream& stream) const
{
    auto frame = makeShared<twtLinkRequirement>();

    uint8_t addrBytes[6];
    for (int i = 0; i < 6; i++) {
        addrBytes[i] = stream.readUint8();
    }
    MacAddress staMac;
    staMac.setAddressBytes(addrBytes);
    frame->setSTAaddress(staMac);
    uint8_t addrBytes2[6];
    for (int i = 0; i < 6; i++) {
        addrBytes2[i] = stream.readUint8();
    }
    MacAddress receiver;
    receiver.setAddressBytes(addrBytes2);
    frame->setReceiver(receiver);

    frame->setDataRate(stream.readUint32Be());
    frame->setWakeTime(stream.readUint32Be());
    frame->setRestriction(stream.readUint8());
    frame->setLatency(stream.readUint32Be());

    frame->setChunkLength(B(25));
    return frame;
}


} // namespace ieee80211
} // namespace inet
