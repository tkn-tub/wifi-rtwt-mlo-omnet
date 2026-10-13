//
//  Created on: 13.04.2025
//      Author: zakrzewski
//

#ifndef MSG_TWTSCHEDULEBEACONSERIALIZER_H_
#define MSG_TWTSCHEDULEBEACONSERIALIZER_H_


#include "inet/common/packet/serializer/FieldsChunkSerializer.h"
#include "inet/common/INETDefs.h"
namespace inet {

namespace ieee80211 {


class TwtScheduleBeaconSerializer : public FieldsChunkSerializer
{
  protected:
    virtual void serialize(MemoryOutputStream& stream, const Ptr<const Chunk>& chunk) const override;
    virtual const Ptr<Chunk> deserialize(MemoryInputStream& stream) const override;

  public:
    TwtScheduleBeaconSerializer() : FieldsChunkSerializer() {}
};

} // namespace ieee80211

} // namespace inet




#endif /* MSG_TWTSCHEDULEBEACONSERIALIZER_H_ */
