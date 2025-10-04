#include "fp16.h"
#include <cmath>

float ConvertFloat16ToFloat(uint16_t float16_bits) {
    int sign = (float16_bits & 0x8000) >> 15;
    int exp = (float16_bits & 0x7c00) >> 10;
    int man = float16_bits & 0x3ff;

    if (exp == 0) {
        return std::powf(-1, sign) * std::powf(2, -14) * man / 1024.0;
    } else if (exp == 31) {
        if (man == 0) {
            return std::powf(-1, sign) * std::numeric_limits<float>::infinity();
        } else {
            return std::nanf(" ");
        }
    } else {
        return std::powf(-1, sign) * std::powf(2, exp - 15) * (1 + man / 1024.0);
    }
}
