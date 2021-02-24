#include "coolmath.h"

float cm_floor(float x) {
    if (x > 0) {
        return (int)x;
    } else if (x < 0) {
        return (int)(x-1);
    } else {
        return 0;
    }
}

float cm_abs(float x) {
    return x > 0 ? x : -x;
}

float cm_frac(float x) {
    return x - cm_floor(x);
}

float cm_lerp(float a, float b, float t) {
    return t*b + (1-t)*a;
}

float cm_unlerp(float a, float b, float t) {
    return (t - a) / (b - a);
}

float cm_remap(float prev_lower, float prev_upper, float new_lower, float new_upper, float a) {
    return cm_lerp(new_lower, new_upper, cm_unlerp(prev_lower, prev_upper, a));
}