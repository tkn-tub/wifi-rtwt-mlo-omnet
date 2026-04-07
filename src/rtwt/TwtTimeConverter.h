//
//  Created on: March 2026
//      Author: Mateusz Zakrzewski (TU Berlin)
//

// This module helps conversions from time units to TWT slots.

#ifndef RTWT_TWTTIMECONVERTER_H_
#define RTWT_TWTTIMECONVERTER_H_

#include "inet/common/INETDefs.h"
#include <cmath>

using omnetpp::simtime_t;

namespace inet {
namespace ieee80211 {

class TwtTimeConverter {
    public:
        static simtime_t calculateTwtInterval(uint16_t intervalMantissa, uint8_t intervalExponent);
        static simtime_t calculateWakeDuration(uint8_t wakeDuration);
        static std::pair<uint16_t, uint8_t> convertMicrosecondsToTwtInterval(uint32_t microseconds);
};

} // namespace ieee80211
} // namespace inet

#endif /* RTWT_TWTTIMECONVERTER_H_ */
