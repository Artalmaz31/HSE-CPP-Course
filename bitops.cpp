#include "bitops.h"

uint64_t SwapBytes(uint64_t value) {
    uint64_t result = 0;
    result |= (value & 0xff00000000000000) >> 56;
    result |= (value & 0x00ff000000000000) >> 40;
    result |= (value & 0x0000ff0000000000) >> 24;
    result |= (value & 0x000000ff00000000) >> 8;
    result |= (value & 0x00000000ff000000) << 8;
    result |= (value & 0x0000000000ff0000) << 24;
    result |= (value & 0x000000000000ff00) << 40;
    result |= (value & 0x00000000000000ff) << 56;
    return result;
}

uint64_t ReverseBits(uint64_t value) {
    uint64_t result = 0;
    uint64_t mask = 1;
    for (int i = 0; i < 64; ++i) {
        if (i < 32) {
            result |= (value & mask) << (63 - i * 2);
        } else {
            result |= (value & mask) >> (i * 2 - 63);
        }
        mask <<= 1;
    }
    return result;
}

uint64_t ReverseBitsInBytes(uint64_t value) {
    uint64_t result = 0;
    uint64_t mask = 1;
    for (int i = 0; i < 64; ++i) {
        if (i % 8 < 4) {
            result |= (value & mask) << (7 - (i % 8) * 2);
        } else {
            result |= (value & mask) >> ((i % 8) * 2 - 7);
        }
        mask <<= 1;
    }
    return result;
}

uint64_t SetBits(uint64_t value, uint64_t offset, uint64_t count, uint64_t bits) {
    uint64_t result = 0;
    uint64_t mask = 1;
    for (int i = 0; i < 64; ++i) {
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
    for (int i = 0; i < 64; ++i) {
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
    for (int i = 0; i < 64; ++i) {
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
    for (int i = 0; i < 64; ++i) {
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
    uint64_t mask = 0x8000000000000000;
    bool flag = true;
    for (int i = 0; i < 64; ++i) {
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
    result |= value << (shift % 64);
    result |= value >> (64 - (shift % 64));
    return result;
}

uint64_t RotateRight(uint64_t value, uint32_t shift) {
    uint64_t result = 0;
    result |= value >> (shift % 64);
    result |= value << (64 - (shift % 64));
    return result;
}

bool IsPowerOfTwo(uint64_t value) {
    uint64_t mask = 1;
    bool flag = false;
    for (int i = 0; i < 64; ++i) {
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
    uint64_t mask = 0x8000000000000000;
    uint64_t result = 1;
    for (int i = 0; i < 64; ++i) {
        if (value & mask) {
            return result << ((value & ~mask) ? 64 - i : 63 - i);
        }
        mask >>= 1;
    }
    return 1;
}

uint64_t AlignDown(uint64_t value, uint64_t alignment) {
    uint64_t result = value;
    uint64_t mask = 1;
    for (int i = 0; i < 64; ++i) {
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
    for (int i = 0; i < 64; ++i) {
        if (alignment & mask) {
            return result;
        }
        result &= ~mask;
        mask <<= 1;
    }
    return 0;
}
