//
//  Created on: 01.03.2025
//      Author: zakrzewski
//

#ifndef MSG_TWTFRAMESERIALIZER_H_
#define MSG_TWTFRAMESERIALIZER_H_


#include "../../rtwt/msg/twtFrame_m.h"
#include "inet/common/packet/serializer/FieldsChunkSerializer.h"
#include "inet/common/INETDefs.h"
namespace inet {

namespace ieee80211 {


class TwtFrameSerializer : public FieldsChunkSerializer
{
  protected:
    virtual void serialize(MemoryOutputStream& stream, const Ptr<const Chunk>& chunk) const override;
    virtual const Ptr<Chunk> deserialize(MemoryInputStream& stream) const override;

  public:
    TwtFrameSerializer() : FieldsChunkSerializer() {}
};

} // namespace ieee80211

} // namespace inet


#endif /* MSG_TWTFRAMESERIALIZER_H_ */
