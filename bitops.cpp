#include "bitops.h"

const int BITS_IN_BYTE = 8;
const int BITS_IN_UINT64 = BITS_IN_BYTE * 8;
const uint64_t FIRST_BIT_IN_UINT64_MASK = 0x8000000000000000;
const uint64_t FIRST_BYTE_IN_UINT64_MASK = 0xff00000000000000;

uint64_t SwapBytes(uint64_t value) {
    uint64_t result = 0;
    uint64_t mask = FIRST_BYTE_IN_UINT64_MASK;
    int shift = BITS_IN_UINT64 - BITS_IN_BYTE;
    for (int i = 0; i < BITS_IN_UINT64 / BITS_IN_BYTE; ++i) {
        result |= (shift > 0) ? (value & mask) >> shift : (value & mask) << -shift;
        mask >>= BITS_IN_BYTE;
        shift -= 2 * BITS_IN_BYTE;
    }
    return result;
}

uint64_t ReverseBits(uint64_t value) {
    uint64_t result = 0;
    uint64_t mask = 1;
    for (int i = 0; i < BITS_IN_UINT64; ++i) {
        if (i < BITS_IN_UINT64 / 2) {
            result |= (value & mask) << ((BITS_IN_UINT64 - 1) - i * 2);
        } else {
            result |= (value & mask) >> (i * 2 - (BITS_IN_UINT64 - 1));
        }
        mask <<= 1;
    }
    return result;
}

uint64_t ReverseBitsInBytes(uint64_t value) {
    uint64_t result = 0;
    uint64_t mask = 1;
    for (int i = 0; i < BITS_IN_UINT64; ++i) {
        if (i % BITS_IN_BYTE < BITS_IN_BYTE / 2) {
            result |= (value & mask) << ((BITS_IN_BYTE - 1) - (i % BITS_IN_BYTE) * 2);
        } else {
            result |= (value & mask) >> ((i % BITS_IN_BYTE) * 2 - (BITS_IN_BYTE - 1));
        }
        mask <<= 1;
    }
    return result;
}

uint64_t SetBits(uint64_t value, uint64_t offset, uint64_t count, uint64_t bits) {
    uint64_t result = 0;
    uint64_t mask = 1;
    for (int i = 0; i < BITS_IN_UINT64; ++i) {
        if (i >= offset && i < offset + count) {
            result |= (bits & (mask >> offset)) << offset;
        } else {
            result |= value & mask;
        }
        mask <<= 1;
    }
    return result;
}

uint64_t ExtractBits(uint64_t value, uint64_t offset, uint64_t count) {
    uint64_t result = 0;
    uint64_t mask = 1;
    for (int i = 0; i < BITS_IN_UINT64; ++i) {
        if (i >= offset && i < offset + count) {
            result |= (value & mask) >> offset;
        }
        mask <<= 1;
    }
    return result;
}

uint32_t CountSetBits(uint64_t value) {
    uint64_t result = 0;
    uint64_t mask = 1;
    for (int i = 0; i < BITS_IN_UINT64; ++i) {
        if (value & mask) {
            ++result;
        }
        mask <<= 1;
    }
    return result;
}

uint32_t CountTrailingZeros(uint64_t value) {
    uint64_t result = 0;
    uint64_t mask = 1;
    bool flag = true;
    for (int i = 0; i < BITS_IN_UINT64; ++i) {
        if (!flag) {
            break;
        }
        if (value & mask) {
            flag = false;
        } else {
            ++result;
        }
        mask <<= 1;
    }
    return result;
}

uint32_t CountLeadingZeros(uint64_t value) {
    uint64_t result = 0;
    uint64_t mask = FIRST_BIT_IN_UINT64_MASK;
    bool flag = true;
    for (int i = 0; i < BITS_IN_UINT64; ++i) {
        if (!flag) {
            break;
        }
        if (value & mask) {
            flag = false;
        } else {
            ++result;
        }
        mask >>= 1;
    }
    return result;
}

uint64_t RotateLeft(uint64_t value, uint32_t shift) {
    uint64_t result = 0;
    result |= value << (shift % BITS_IN_UINT64);
    result |= value >> (BITS_IN_UINT64 - (shift % BITS_IN_UINT64));
    return result;
}

uint64_t RotateRight(uint64_t value, uint32_t shift) {
    uint64_t result = 0;
    result |= value >> (shift % BITS_IN_UINT64);
    result |= value << (BITS_IN_UINT64 - (shift % BITS_IN_UINT64));
    return result;
}

bool IsPowerOfTwo(uint64_t value) {
    uint64_t mask = 1;
    bool flag = false;
    for (int i = 0; i < BITS_IN_UINT64; ++i) {
        if (value & mask) {
            if (flag) {
                return false;
            } else {
                flag = true;
            }            
        }
        mask <<= 1;
    }
    return flag;
}

uint64_t RoundUpToPowerOfTwo(uint64_t value) {
    if (!(~value)) {
        return 0;
    }
    uint64_t mask = FIRST_BIT_IN_UINT64_MASK;
    uint64_t result = 1;
    for (int i = 0; i < BITS_IN_UINT64; ++i) {
        if (value & mask) {
            return result << ((value & ~mask) ? BITS_IN_UINT64 - i : (BITS_IN_UINT64 - 1) - i);
        }
        mask >>= 1;
    }
    return 1;
}

uint64_t AlignDown(uint64_t value, uint64_t alignment) {
    uint64_t result = value;
    uint64_t mask = 1;
    for (int i = 0; i < BITS_IN_UINT64; ++i) {
        if (alignment & mask) {
            return result;
        }
        result &= ~mask;
        mask <<= 1;
    }
    return 0;
}

uint64_t AlignUp(uint64_t value, uint64_t alignment) {
    uint64_t result = value + alignment - 1;
    uint64_t mask = 1;
    for (int i = 0; i < BITS_IN_UINT64; ++i) {
        if (alignment & mask) {
            return result;
        }
        result &= ~mask;
        mask <<= 1;
    }
    return 0;
}
