#include "fp16.h"
#include <cmath>

const int SIGN_BIT_MASK = 0x8000;
const int SIGN_BIT_SHIFT = 15;
const int EXP_BITS_MASK = 0x7c00;
const int EXP_BITS_SHIFT = 10;
const int MAN_BITS_MASK = 0x3ff;
const int EXP_MIN = -14;
const int EXP_MAX = 31;
const int EXP_BIAS = 15;
const double MAN_DENOM = 1024.0;

float ConvertFloat16ToFloat(uint16_t float16_bits) {
    int sign = (float16_bits & SIGN_BIT_MASK) >> SIGN_BIT_SHIFT;
    int exp = (float16_bits & EXP_BITS_MASK) >> EXP_BITS_SHIFT;
    int man = float16_bits & MAN_BITS_MASK;

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
