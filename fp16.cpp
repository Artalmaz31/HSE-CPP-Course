#include "fp16.h"
#include <cmath>

namespace {
    constexpr size_t SIGN_BIT_MASK = 0x8000;
    constexpr size_t SIGN_BIT_SHIFT = 15;
    constexpr size_t EXP_BITS_MASK = 0x7c00;
    constexpr size_t EXP_BITS_SHIFT = 10;
    constexpr size_t MAN_BITS_MASK = 0x3ff;
    constexpr size_t EXP_MIN = -14;
    constexpr size_t EXP_MAX = 31;
    constexpr size_t EXP_BIAS = 15;
    constexpr double MAN_DENOM = 1024.0;
}

float ConvertFloat16ToFloat(uint16_t float16_bits) {
    size_t sign = (float16_bits & SIGN_BIT_MASK) >> SIGN_BIT_SHIFT;
    size_t exp = (float16_bits & EXP_BITS_MASK) >> EXP_BITS_SHIFT;
    size_t man = float16_bits & MAN_BITS_MASK;

    if (exp == 0) {
        return std::powf(-1, sign) * std::powf(2, EXP_MIN) * man / MAN_DENOM;
    } else if (exp == EXP_MAX) {
        if (man == 0) {
            return std::powf(-1, sign) * std::numeric_limits<float>::infinity();
        } else {
            return std::nanf(" ");
        }
    } else {
        return std::powf(-1, sign) * std::powf(2, exp - EXP_BIAS) * (1 + man / MAN_DENOM);
    }
}
