//
//  Created on: March 2026
//      Author: Mateusz Zakrzewski (TU Berlin)
//

// This module helps conversions from time units to TWT slots.

#include "inet/common/INETDefs.h"
#include <cmath>

#include "TwtTimeConverter.h"

namespace inet {
namespace ieee80211 {

// Calculates sim time from interval mantissa and exponent
simtime_t TwtTimeConverter::calculateTwtInterval(uint16_t intervalMantissa, uint8_t intervalExponent) {
    // IEEE 802.11 formula: Interval = Mantissa × 2^Exponent (in microseconds)
    double intervalMicroseconds = intervalMantissa * std::pow(2, intervalExponent);
    return SimTime(intervalMicroseconds, SimTimeUnit::SIMTIME_US);
}

// convert microseconds to TWT interval
std::pair<uint16_t, uint8_t> TwtTimeConverter::convertMicrosecondsToTwtInterval(uint32_t microseconds) {
    uint32_t intervalUs = microseconds;
    uint8_t exponent = 0;
    while (intervalUs > 0xFFFF) {
        intervalUs >>= 1;
        exponent++;
    }
    uint16_t mantissa = static_cast<uint16_t>(intervalUs);
    return std::make_pair(mantissa, exponent);
}

simtime_t TwtTimeConverter::calculateWakeDuration(uint8_t wakeDuration) {
    return SimTime(wakeDuration * 256, SimTimeUnit::SIMTIME_US);
}

} // namespace ieee80211
} // namespace inet

