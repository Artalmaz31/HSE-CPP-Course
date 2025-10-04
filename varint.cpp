#include "varint.h"

size_t DecodeVarint(const uint8_t* data, size_t size, uint64_t& result) {
    if (size == 0 || size > 10 || (size == 10 && data[size - 1] > 1)) {
        return 0;
    }

    uint64_t temp_result = 0;
    int shift = 0;

    for (size_t i = 0; i < size; ++i) {
        temp_result |= (data[i] & 127) << shift;
        shift += 7;

        if (!(data[i] & 128)) {
            result = temp_result;
            return i + 1;
        }
    }
    return 0;
}
