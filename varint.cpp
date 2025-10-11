#include "varint.h"

namespace {
    constexpr size_t MAX_SIZE = 10;
    constexpr size_t BITS_IN_BYTE = 8;
    constexpr size_t FIRST_BIT_MASK = 128;
}

size_t DecodeVarint(const uint8_t* data, size_t size, uint64_t& result) {
    if (data == nullptr || size == 0 || size > MAX_SIZE || (size == MAX_SIZE && data[size - 1] > 1)) {
        return 0;
    }

    uint64_t temp_result = 0;
    size_t shift = 0;

    for (size_t i = 0; i < size; ++i) {
        temp_result |= (data[i] & ~FIRST_BIT_MASK) << shift;
        shift += BITS_IN_BYTE - 1;

        if (!(data[i] & FIRST_BIT_MASK)) {
            result = temp_result;
            return i + 1;
        }
    }
    return 0;
}
