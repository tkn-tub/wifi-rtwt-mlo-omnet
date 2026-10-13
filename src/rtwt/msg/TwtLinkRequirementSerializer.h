//
//  Created on: 01.03.2025
//      Author: zakrzewski
//

#ifndef MSG_TWTLINKREQUIREMENTSERIALIZER_H_
#define MSG_TWTLINKREQUIREMENTSERIALIZER_H_


#include "inet/common/packet/serializer/FieldsChunkSerializer.h"
#include "inet/common/INETDefs.h"
namespace inet {

namespace ieee80211 {


class TwtLinkRequirementSerializer : public FieldsChunkSerializer
{
  protected:
    virtual void serialize(MemoryOutputStream& stream, const Ptr<const Chunk>& chunk) const override;
    virtual const Ptr<Chunk> deserialize(MemoryInputStream& stream) const override;

  public:
    TwtLinkRequirementSerializer() : FieldsChunkSerializer() {}
};

} // namespace ieee80211

} // namespace inet


#endif /* MSG_TWTLINKREQUIREMENTSERIALIZER_H_ */
