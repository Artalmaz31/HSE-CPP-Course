#include "safe_arithmetic.h"
#include <limits>

bool SafeAdd(int64_t a, int64_t b, int64_t& result) {    
    if (b > 0 && a > INT64_MAX - b) {
        return false;
    } else if (b < 0 && a < INT64_MIN - b) {
        return false;
    }

    result = a + b;
    return true;
}

bool SafeSubtract(int64_t a, int64_t b, int64_t& result) {
    if (b > 0 && a < INT64_MIN + b) {
        return false;
    } else if (b < 0 && a > INT64_MAX + b) {
        return false;
    }

    result = a - b;
    return true;
}

bool SafeMultiply(int64_t a, int64_t b, int64_t& result) {
    if (b > 0 && (a < INT64_MIN / b || a > INT64_MAX / b)) {
        return false;
    } else if (b < 0 && (a < INT64_MAX / b || a > INT64_MIN / b)) {
        return false;
    }

    result = a * b;
    return true;
}

bool SafeDivide(int64_t a, int64_t b, int64_t& result) {
    if (b == 0) {
        return false;
    } else if (b == -1 && a == INT64_MIN) {
        return false;
    }
    
    result = a / b;
    return true;
}
