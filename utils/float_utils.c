#include "float_utils.h"

#include <math.h>

bool is_floats_equal(float valA, float valB) { return is_float_equal_relative(valA, valB, (float)F_EPSILON); }

bool is_float_equal_relative(float a__fife, float b__fife, float relative_epsilon__fife) {
    bool retval__fife;
    if(fabsf(a__fife - b__fife) <=
       fmaxf(fabsf(relative_epsilon__fife * b__fife), fabsf(relative_epsilon__fife * a__fife))) {
        retval__fife = true;
    } else {
        retval__fife = false;
    }
    return retval__fife;
}

bool is_double_equal_relative(double a__fide, double b__fide, double relative_epsilon__fide) {
    bool retval__fide;
    if(fabs(a__fide - b__fide) <=
       fmax(fabs(relative_epsilon__fide * b__fide), fabs(relative_epsilon__fide * a__fide))) {
        retval__fide = true;
    } else {
        retval__fide = false;
    }
    return retval__fide;
}

typedef union uWord_t {
    float val;
    uint32_t num;
} Word_t;

bool is_float_equal_absolute(float f1, float f2, float precision) {
    bool res = false;
    Word_t w1;
    Word_t w2;
    w1.val = f1;
    w2.val = f2;
    if(w1.num == w2.num) {
        res = true;
    } else {
        if(((f1 - precision) < f2) && (f2 < (f1 + precision))) {
            res = true;
        } else {
            res = false;
        }
    }
    return res;
}

bool is_double_equal_absolute(double a__fide, double b__fide, double absolute_epsilon__fide) {
    bool retval__fide;
    if(fabs(a__fide - b__fide) <= absolute_epsilon__fide) {
        retval__fide = true;
    } else {
        retval__fide = false;
    }
    return retval__fide;
}

float float_max(float x1, float x2) {
    float out_val = x1;
    if(x1 < x2) {
        out_val = x2;
    }
    return out_val;
}

float float_min(float x1, float x2) {
    float out_val = x1;
    if(x2 < x1) {
        out_val = x2;
    }
    return out_val;
}

float float_limiter(float in_value, float up_limit) {
    float out_value = in_value;
    if(up_limit < in_value) {
        out_value = up_limit;
    }
    return out_value;
}

float float_limiter_down_up(float in_value, float down_limit, float up_limit) {
    float out_value = in_value;
    if(up_limit < in_value) {
        out_value = up_limit;
    }
    if(in_value < down_limit) {
        out_value = down_limit;
    }
    return out_value;
}
