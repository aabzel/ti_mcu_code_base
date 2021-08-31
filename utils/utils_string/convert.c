#include "convert.h"

#include <ctype.h>
#include <float.h>
#include <limits.h>
#include <stdlib.h>
#include <math.h>

static const char symbols[] = "FEDCBA9876543210123456789ABCDEF";

static bool try_hex_char_to_u8(uint8_t hex_char, uint8_t* hex_char_to_u8_value);
static bool try_dec_char_to_u8(uint8_t dec_char, uint8_t* dec_char_to_u8_value);
static bool is_signed(const char first_str_char, int32_t* first_digit_index);
static bool get_str_len(const char char_str[], int32_t* str_len);
static bool rx_strtod(const char str[], const char** endptr, double_t* result);
static bool is_hex_digit(const char character);
static bool is_true(const char* true_str_to_check);
static bool is_false(const char* false_str_to_check);
static void dtoa_normal(double_t double_data, int32_t double_precision, char double_stringified[]);
static void ftoa_normal(float_t float_data, int32_t float_precision, char float_stringified[]);

static const float_t rounders[(MAX_PRECISION + 1U)] = {
    0.5f,          /* 0 */
    0.05f,         /* 1 */
    0.005f,        /* 2 */
    0.0005f,       /* 3 */
    0.00005f,      /* 4 */
    0.000005f,     /* 5 */
    0.0000005f,    /* 6 */
    0.00000005f,   /* 7 */
    0.000000005f,  /* 8 */
    0.0000000005f, /* 9 */
    0.00000000005f /* 10 */
};

/* STRING TO 64 BIT
 * ****************************************************************************
 */

bool try_str2uint64(const char u64_str[], uint64_t* u64_value) {
    bool res = false;
    if(NULL != u64_str) {
        if(NULL != u64_value) {
            int32_t u64_str_len = (int32_t)strlen(u64_str);
            res = try_strl2uint64(u64_str, u64_str_len, u64_value);
        }
    }
    return res;
}

bool try_str2int64(const char s64_str[], int64_t* s64_value) {
    bool res = false;
    if(NULL != s64_str) {
        if(NULL != s64_value) {
            int32_t s64_str_len = (int32_t)strlen(s64_str);
            res = try_strl2int64(s64_str, s64_str_len, s64_value);
        }
    }
    return res;
}

bool try_strl2uint64(const char u64l_str[], int32_t u64l_str_len, uint64_t* u64l_value) {
    bool u64l_success = true;
    bool u64l_str_not_empty = true;
    int32_t u64l_len = u64l_str_len;
    *u64l_value = 0U;
    if(NULL != u64l_str) {
        if(NULL != u64l_value) {
            u64l_str_not_empty = get_str_len(u64l_str, &u64l_len);
            if(u64l_str_not_empty == false) {
                u64l_success = false;
            }

            if(u64l_success == true) {
                uint8_t num_shift = 0U;
                if(is_hex_str(u64l_str, u64l_len, &num_shift) == true) {
                    u64l_success =
                        try_strl2uint64_hex(&u64l_str[num_shift], u64l_len - ((int32_t)num_shift), u64l_value);
                } else {
                    u64l_success = try_strl2uint64_dec(u64l_str, u64l_len, u64l_value);
                }
            }
        }
    } else {
        u64l_success = false;
    }

    return u64l_success;
}

bool try_strl2int64(const char s64l_str[], int32_t s64l_str_len, int64_t* s64l_value) {
    bool s64l_success = true;
    bool s64l_str_not_empty = true;
    int32_t s64l_len = s64l_str_len;
    if(NULL != s64l_str) {
        if(NULL != s64l_value) {
            *s64l_value = 0;
            s64l_str_not_empty = get_str_len(s64l_str, &s64l_len);
            if(s64l_str_not_empty == false) {
                s64l_success = false;
            }

            if(s64l_success == true) {
                uint8_t num_shift = 0U;
                if(is_hex_str(s64l_str, s64l_len, &num_shift) == true) {
                    s64l_success =
                        try_strl2int64_hex(&s64l_str[num_shift], s64l_len - ((int32_t)num_shift), s64l_value);
                } else {
                    s64l_success = try_strl2int64_dec(s64l_str, s64l_len, s64l_value);
                }
            }
        } else {
            s64l_success = false;
        }
    } else {
        s64l_success = false;
    }

    return s64l_success;
}

bool try_strl2uint64_hex(const char u64_hex_str[], int32_t u64_hex_str_len, uint64_t* u64_hex_value) {
    bool u64_hex_success = true;
    bool u64_str_not_empty = true;
    uint64_t u64_hex_result = 0U;
    int32_t u64_hex_str_index;
    int32_t u64_hex_len = u64_hex_str_len;
    if((NULL != u64_hex_str) && (NULL != u64_hex_value)) {
        u64_str_not_empty = get_str_len(u64_hex_str, &u64_hex_len);
        if(u64_str_not_empty == false) {
            u64_hex_success = false;
        }

        if(u64_hex_len > (int32_t)MAX_INT64_STR_LEN_16) {
            u64_hex_success = false;
        }

        if(u64_hex_success == true) {
            for(u64_hex_str_index = 0; u64_hex_str_index < u64_hex_len; u64_hex_str_index++) {
                uint8_t u64_hex_str_char = (uint8_t)u64_hex_str[u64_hex_str_index];
                uint8_t u64_hex_str_number = 0U;

                if(true == try_hex_char_to_u8(u64_hex_str_char, &u64_hex_str_number)) {
                    u64_hex_result = (u64_hex_result * 16U) + u64_hex_str_number;
                } else {
                    u64_hex_success = false;
                    u64_hex_result = 0U;
                    break;
                }
            }
        }

        *u64_hex_value = u64_hex_result;
    } else {
        u64_hex_success = false;
    }
    return u64_hex_success;
}

bool try_strl2int64_hex(const char s64_hex_str[], int32_t s64_hex_str_len, int64_t* s64_hex_value) {
    bool s64_hex_success = true;
    bool s64_str_not_empty = true;
    int64_t s64_hex_result = 0;
    int32_t s64_hex_str_index;
    int32_t s64_hex_len = s64_hex_str_len;
    if((NULL != s64_hex_str) && (NULL != s64_hex_value)) {
        s64_str_not_empty = get_str_len(s64_hex_str, &s64_hex_len);
        if(s64_str_not_empty == false) {
            s64_hex_success = false;
        }

        if(s64_hex_len > (int32_t)MAX_INT64_STR_LEN_16) {
            s64_hex_success = false;
        }

        if(s64_hex_success == true) {
            for(s64_hex_str_index = 0; s64_hex_str_index < s64_hex_len; s64_hex_str_index++) {
                uint8_t s64_hex_str_char = (uint8_t)s64_hex_str[s64_hex_str_index];
                uint8_t s64_hex_str_number = 0U;
                s64_hex_success = try_hex_char_to_u8(s64_hex_str_char, &s64_hex_str_number);

                if(s64_hex_success == true) {
                    s64_hex_result = (s64_hex_result * 16) + (int64_t)s64_hex_str_number;
                    if(s64_hex_result > (INT64_MAX / 16)) {
                        s64_hex_success = false;
                    }
                }

                if(s64_hex_success == false) {
                    s64_hex_result = 0;
                    break;
                }
            }
        }

        *s64_hex_value = s64_hex_result;
    } else {
        s64_hex_success = false;
    }
    return s64_hex_success;
}

bool try_strl2uint64_dec(const char u64_dec_str[], int32_t u64_dec_str_len, uint64_t* u64_dec_value) {
    bool u64l_dec_success = true;
    uint64_t u64l_dec_result = 0U;
    int32_t u64l_dec_str_index = 0;

    if(true == is_signed(u64_dec_str[0], &u64l_dec_str_index)) {
        u64l_dec_success = false;
    } else {
        for(; u64l_dec_str_index < u64_dec_str_len; u64l_dec_str_index++) {
            uint8_t u64l_dec_str_char = (uint8_t)u64_dec_str[u64l_dec_str_index];
            uint8_t u64l_dec_str_number = 0U;
            uint64_t u64l_dec_temp_value = 0U;

            u64l_dec_success = try_dec_char_to_u8(u64l_dec_str_char, &u64l_dec_str_number);
            if(u64l_dec_success == true) {
                u64l_dec_temp_value = (u64l_dec_result * 10U) + u64l_dec_str_number;
                if(u64l_dec_temp_value < u64l_dec_result) {
                    u64l_dec_success = false;
                }
            }
            if(u64l_dec_success == false) {
                u64l_dec_result = 0U;
                break;
            }
            u64l_dec_result = u64l_dec_temp_value;
        }
    }

    *u64_dec_value = u64l_dec_result;
    return u64l_dec_success;
}

bool try_strl2int64_dec(const char s64_dec_str[], int32_t s64_dec_str_len, int64_t* s64_dec_value) {
    bool s64l_dec_success = true;
    bool s64l_dec_signed = false;
    int64_t s64l_dec_result = 0;
    int32_t s64l_dec_str_index = 0;
    if((NULL != s64_dec_str) && (NULL != s64_dec_value)) {
        s64l_dec_signed = is_signed(s64_dec_str[0], &s64l_dec_str_index);
        if((s64l_dec_signed == true) && (s64_dec_str_len < 2)) {
            s64l_dec_success = false;
        }

        if(s64l_dec_success == true) {
            for(; s64l_dec_str_index < s64_dec_str_len; s64l_dec_str_index++) {
                uint8_t s64_dec_str_char = (uint8_t)s64_dec_str[s64l_dec_str_index];
                uint8_t s64_dec_str_number = 0U;
                int64_t s64_dec_temp_value = 0;

                s64l_dec_success = try_dec_char_to_u8(s64_dec_str_char, &s64_dec_str_number);
                if(s64l_dec_success == true) {
                    s64_dec_temp_value = (s64l_dec_result * 10) + (int64_t)s64_dec_str_number;
                    if(s64_dec_temp_value < s64l_dec_result) {
                        s64l_dec_success = false;
                    }
                }

                if(s64l_dec_success == false) {
                    s64l_dec_result = 0;
                    break;
                }

                s64l_dec_result = s64_dec_temp_value;
            }
        }

        if(s64l_dec_signed == true) {
            *s64_dec_value = -s64l_dec_result;
        } else {
            *s64_dec_value = s64l_dec_result;
        }
    } else {
        s64l_dec_success = false;
    }
    return s64l_dec_success;
}

/* STRING TO 32 BIT
 * ****************************************************************************
 */

bool try_str2uint32(const char u32_str[], uint32_t* u32_value) {
    bool res = false;
    if((NULL != u32_str) && (NULL != u32_value)) {
        int32_t u32_str_len = (int32_t)strlen(u32_str);
        res = try_strl2uint32(u32_str, u32_str_len, u32_value);
    }
    return res;
}

bool try_str2int32(const char s32_str[], int32_t* s32_value) {
    bool res = false;
    if((NULL != s32_str) && (NULL != s32_value)) {
        int32_t s32_str_len = (int32_t)strlen(s32_str);
        res = try_strl2int32(s32_str, s32_str_len, s32_value);
    }
    return res;
}

bool try_strl2uint32(const char u32l_str[], int32_t u32l_str_len, uint32_t* u32l_value) {
    bool u32l_success = true;
    bool u32l_str_not_empty = true;
    int32_t u32l_len = u32l_str_len;
    if((NULL != u32l_str) && (NULL != u32l_value)) {
        (*u32l_value) = 0U;
        u32l_str_not_empty = get_str_len(u32l_str, &u32l_len);
        if(u32l_str_not_empty == false) {
            u32l_success = false;
        } else {
            uint8_t num_shift = 0U;
            if(is_hex_str(u32l_str, u32l_len, &num_shift) == true) {
                u32l_success = try_strl2uint32_hex(&u32l_str[num_shift], u32l_len - ((int32_t)num_shift), u32l_value);
            } else {
                u32l_success = try_strl2uint32_dec(u32l_str, u32l_len, u32l_value);
            }
        }
    } else {
        u32l_success = false;
    }
    return u32l_success;
}

bool try_strl2int32(const char s32l_str[], int32_t s32l_str_len, int32_t* s32l_value) {
    bool s32l_success = true;
    bool s32l_str_not_empty = true;
    int32_t s32l_len = s32l_str_len;
    if(NULL != s32l_value) {
        *s32l_value = 0;
    }

    s32l_str_not_empty = get_str_len(s32l_str, &s32l_len);
    if(s32l_str_not_empty == false) {
        s32l_success = false;
    }

    if(s32l_success == true) {
        uint8_t out_shift = 0U;
        if(is_hex_str(s32l_str, s32l_len, &out_shift) == true) {
            s32l_success = try_strl2int32_hex(&s32l_str[out_shift], s32l_len - ((int32_t)out_shift), s32l_value);
        } else {
            s32l_success = try_strl2int32_dec(s32l_str, s32l_len, s32l_value);
        }
    }

    return s32l_success;
}

bool try_str2uint32_hex(const char u32_hex_str[], uint32_t* u32_hex_value) {
    bool res = false;
    if((NULL != u32_hex_str) && (NULL != u32_hex_value)) {
        int32_t u32_hex_str_len = (int32_t)strlen(u32_hex_str);
        res = try_strl2uint32_hex(u32_hex_str, u32_hex_str_len, u32_hex_value);
    }
    return res;
}

bool try_strl2uint32_hex(const char u32l_hex_str[], int32_t u32l_hex_str_len, uint32_t* u32l_hex_value) {
    uint64_t u32l_hex_result = 0U;
    bool u32l_hex_success = try_strl2uint64_hex(u32l_hex_str, u32l_hex_str_len, &u32l_hex_result);

    if((u32l_hex_success == true) && (u32l_hex_result <= (uint64_t)UINT32_MAX)) {
        *u32l_hex_value = (uint32_t)u32l_hex_result;
    } else {
        u32l_hex_success = false;
        *u32l_hex_value = 0U;
    }

    return u32l_hex_success;
}

bool try_strl2int32_hex(const char s32l_hex_str[], int32_t s32l_hex_str_len, int32_t* s32l_hex_value) {
    int64_t s32l_hex_result = 0;
    bool s32l_hex_success = try_strl2int64_hex(s32l_hex_str, s32l_hex_str_len, &s32l_hex_result);

    if((s32l_hex_success == true) && (s32l_hex_result <= INT32_MAX) && (s32l_hex_result >= INT32_MIN)) {
        *s32l_hex_value = (int32_t)s32l_hex_result;
    } else {
        s32l_hex_success = false;
        *s32l_hex_value = 0;
    }

    return s32l_hex_success;
}

bool try_strl2uint32_dec(const char u32_dec_str[], int32_t u32_dec_str_len, uint32_t* u32_dec_value) {
    uint64_t u32l_dec_result = 0U;
    bool u32l_dec_success = try_strl2uint64_dec(u32_dec_str, u32_dec_str_len, &u32l_dec_result);

    if((u32l_dec_success == true) && (u32l_dec_result <= UINT32_MAX)) {
        *u32_dec_value = (uint32_t)u32l_dec_result;
    } else {
        u32l_dec_success = false;
        *u32_dec_value = 0U;
    }

    return u32l_dec_success;
}

bool try_strl2int32_dec(const char s32_dec_str[], int32_t s32_dec_str_len, int32_t* s32_dec_value) {
    int64_t s32l_dec_result = 0;
    bool s32l_dec_success = try_strl2int64_dec(s32_dec_str, s32_dec_str_len, &s32l_dec_result);

    if((s32l_dec_success == true) && (s32l_dec_result <= INT32_MAX) && (s32l_dec_result >= INT32_MIN)) {
        *s32_dec_value = (int32_t)s32l_dec_result;
    } else {
        s32l_dec_success = false;
        *s32_dec_value = 0;
    }

    return s32l_dec_success;
}

/* STRING TO 16 BIT
 * ****************************************************************************
 */

bool try_str2uint16(const char u16_str[], uint16_t* u16_value) {
    bool res = false;
    if((NULL != u16_str) && (NULL != u16_value)) {
        int32_t u16_str_len = (int32_t)strlen(u16_str);
        res = try_strl2uint16(u16_str, u16_str_len, u16_value);
    }
    return res;
}

bool try_str2int16(const char s16_str[], int16_t* s16_value) {
    int32_t s16_str_len = (int32_t)strlen(s16_str);
    return try_strl2int16(s16_str, s16_str_len, s16_value);
}

bool try_strl2uint16(const char u16l_str[], int32_t u16l_str_len, uint16_t* u16l_value) {
    uint32_t u16l_result = 0U;
    bool u16l_success = try_strl2uint32(u16l_str, u16l_str_len, &u16l_result);

    if((u16l_success == true) && (u16l_result <= (uint32_t)UINT16_MAX)) {
        *u16l_value = (uint16_t)u16l_result;
    } else {
        u16l_success = false;
        *u16l_value = 0U;
    }

    return u16l_success;
}

bool try_strl2int16(const char s16l_str[], int32_t s16l_str_len, int16_t* s16l_value) {
    int32_t s16l_result = 0;
    bool s16l_success = try_strl2int32(s16l_str, s16l_str_len, &s16l_result);

    if((s16l_success == true) && (s16l_result <= INT16_MAX) && (s16l_result >= INT16_MIN)) {
        *s16l_value = (int16_t)s16l_result;
    } else {
        s16l_success = false;
        *s16l_value = 0;
    }

    return s16l_success;
}

/* STRING TO 8 BIT
 * *****************************************************************************
 */

bool try_str2uint8(const char u8_str[], uint8_t* u8_value) {
    bool res = false;
    if((NULL != u8_str) && (NULL != u8_value)) {
        int32_t u8_str_len = (int32_t)strlen(u8_str);
        res = try_strl2uint8(u8_str, u8_str_len, u8_value);
    } /*Otherwise reboot will occur*/
    return res;
}

bool try_str2int8(const char s8_str[], int8_t* s8_value) {
    int32_t s8_str_len = (int32_t)strlen(s8_str);
    return try_strl2int8(s8_str, s8_str_len, s8_value);
}

bool try_strl2uint8(const char u8l_str[], int32_t u8l_str_len, uint8_t* u8l_value) {
    uint32_t u8l_result = 0U;
    bool u8l_success = try_strl2uint32(u8l_str, u8l_str_len, &u8l_result);

    if((u8l_success == true) && (u8l_result <= (uint32_t)UINT8_MAX)) {
        *u8l_value = (uint8_t)u8l_result;
    } else {
        u8l_success = false;
        *u8l_value = 0U;
    }

    return u8l_success;
}

bool try_strl2int8(const char s8l_str[], int32_t s8l_str_len, int8_t* s8l_value) {
    int32_t s8l_result = 0;
    bool s8l_success = try_strl2int32(s8l_str, s8l_str_len, &s8l_result);

    if((s8l_success == true) && (s8l_result <= INT8_MAX) && (s8l_result >= INT8_MIN)) {
        *s8l_value = (int8_t)s8l_result;
    } else {
        s8l_success = false;
        *s8l_value = 0;
    }

    return s8l_success;
}

bool try_strl2uint8_hex(const char u8l_hex_str[], int32_t u8l_hex_str_len, uint8_t* u8l_hex_value) {
    bool u8l_hex_success = false;
    if(NULL != u8l_hex_str) {
        uint64_t u8l_hex_result = 0U;
        u8l_hex_success = try_strl2uint64_hex(u8l_hex_str, u8l_hex_str_len, &u8l_hex_result);

        if((u8l_hex_success == true) && (u8l_hex_result <= (uint64_t)UINT8_MAX)) {
            *u8l_hex_value = (uint8_t)u8l_hex_result;
        } else {
            u8l_hex_success = false;
            *u8l_hex_value = 0U;
        }
    }

    return u8l_hex_success;
}

bool try_strl2uint16_hex(const char u16l_hex_str[], int32_t u16l_hex_str_len, uint16_t* u16l_hex_value) {
    uint64_t u16l_hex_result = 0U;
    bool u16l_hex_success = try_strl2uint64_hex(u16l_hex_str, u16l_hex_str_len, &u16l_hex_result);

    if((u16l_hex_success == true) && (u16l_hex_result <= (uint64_t)UINT16_MAX)) {
        *u16l_hex_value = (uint16_t)u16l_hex_result;
    } else {
        u16l_hex_success = false;
        *u16l_hex_value = 0U;
    }

    return u16l_hex_success;
}

bool try_strl2int8_hex(const char s8l_hex_str[], int32_t s8l_hex_str_len, int8_t* s8l_hex_value) {
    int64_t s8l_hex_result = 0;
    bool s8l_hex_success = try_strl2int64_hex(s8l_hex_str, s8l_hex_str_len, &s8l_hex_result);

    if((s8l_hex_success == true) && (s8l_hex_result <= INT8_MAX) && (s8l_hex_result >= INT8_MIN)) {
        *s8l_hex_value = (int8_t)s8l_hex_result;
    } else {
        s8l_hex_success = false;
        *s8l_hex_value = 0;
    }

    return s8l_hex_success;
}

/* STRING TO BOOL
 * ******************************************************************************
 */

bool try_str2bool(const char bool_str[], bool* bool_value) {
    uint8_t bool_tmp_result[6] = {0U, 0U, 0U, 0U, 0U, 0U};
    int32_t bool_str_len = -1;
    int32_t bool_str_index;
    bool bool_success = true;
    bool bool_str_not_empty = true;
    if((NULL != bool_str) && (NULL != bool_value)) {
        bool_str_not_empty = get_str_len(bool_str, &bool_str_len);
        if((bool_str_not_empty == false) || (bool_str_len > 5)) {
            bool_success = false;
        }

        if(bool_success == true) {
            for(bool_str_index = 0; bool_str_index < bool_str_len; bool_str_index++) {
                uint8_t bool_str_char = (uint8_t)bool_str[bool_str_index];
                bool_tmp_result[bool_str_index] = (uint8_t)tolower((int32_t)bool_str_char);
            }
            bool_tmp_result[bool_str_len] = 0U;

            if(is_true((const char*)bool_tmp_result) == true) {
                *bool_value = true;
            } else if(is_false((const char*)bool_tmp_result) == true) {
                *bool_value = false;
            } else {
                bool_success = false;
            }
        }
    } else {
        bool_success = false;
    }
    return bool_success;
}

/* STRING TO FLOATING
 * ************************************************************************** */

bool try_str2float(const char float_str[], float_t* float_value) {
    bool float_success = false;

    if((NULL != float_value) && (NULL != float_str)) {
        const char* float_ptr = NULL;
        double_t float_temp_value = 0.0;
        float_success = rx_strtod(float_str, &float_ptr, &float_temp_value);

        if((float_success == true) && (*float_ptr != '\0')) {
            float_success = false;
        } else {
            bool is_positive = (float_temp_value > 0.0);
            bool is_negative = (float_temp_value < 0.0);
            bool hi_pos_bound_exceeded = (float_temp_value > FLT_MAX);
            bool lo_pos_bound_exceeded = (float_temp_value < FLT_MIN);
            bool lo_neg_bound_exceeded = (float_temp_value < -FLT_MAX);
            if((is_positive == true) && ((hi_pos_bound_exceeded == true) || (lo_pos_bound_exceeded == true))) {
                float_success = false;
            } else if((is_negative == true) && (lo_neg_bound_exceeded == true)) {
                float_success = false;
            } else {
                *float_value = (float_t)float_temp_value;
            }
        }
    } else {
        float_success = false;
    }
    return float_success;
}

bool try_str2double(const char double_str[], double_t* double_value) {
    bool double_success = false;
    const char* double_ptr = NULL;
    if(NULL != double_str) {
        if(NULL != double_value) {
            double_success = rx_strtod(double_str, &double_ptr, double_value);
            if((double_success == true) && (*double_ptr != '\0')) {
                double_success = false;
            }
        }
    }
    return double_success;
}

bool try_strl2double(const char double_str[], int32_t str_len, double_t* double_value) {
    bool double_success = false;
    char tempStr[30] = "";
    memset(tempStr, 0x00, sizeof(tempStr));
    memcpy(tempStr, double_str, str_len);
    double_success = try_str2double(tempStr, double_value);
    return double_success;
}

const char* ltoa32_(int32_t s32_data, char s32_stringified[], uint8_t s32_base, uint32_t* s32_len) {
    char s32_reverse_str[MAX_INT32_STR_LEN_10 + 1U];
    uint32_t s32_reverse_str_index = 0U;
    uint32_t s32_result_str_index = 0U;
    uint32_t s32_result_str_len = 0U;
    int32_t s32_symbol_index = 0;
    int32_t s32_temp_value = 0;
    int32_t s32_d = s32_data;

    do {
        s32_temp_value = s32_d;
        s32_d = s32_d / (int32_t)s32_base;
        s32_symbol_index = 15 + (s32_temp_value - (s32_d * (int32_t)s32_base));
        s32_reverse_str[s32_reverse_str_index] = symbols[s32_symbol_index];
        s32_reverse_str_index++;
    } while(s32_d != 0);

    /* Apply negative sign */
    if(s32_temp_value < 0) {
        s32_reverse_str[s32_reverse_str_index] = '-';
        s32_reverse_str_index++;
    }

    s32_result_str_len = s32_reverse_str_index;

    if(s32_len != NULL) {
        *s32_len = s32_result_str_len;
    }

    s32_reverse_str[s32_reverse_str_index] = '\0';
    s32_reverse_str_index--;

    for(s32_result_str_index = 0U; s32_result_str_index < s32_result_str_len; s32_result_str_index++) {
        s32_stringified[s32_result_str_index] = s32_reverse_str[s32_reverse_str_index];
        s32_reverse_str_index--;
    }

    s32_stringified[s32_result_str_index] = '\0';

    return s32_stringified;
}

const char* ltoa64_(int64_t s64_data, char s64_stringified[], uint8_t s64_base, uint32_t* s64_len) {
    char s64_reverse_str[MAX_INT64_STR_LEN_10 + 1U];
    uint32_t s64_reverse_str_index = 0U;
    uint32_t s64_result_str_index = 0U;
    uint32_t s64_result_str_len = 0U;
    int64_t s64_temp_value = 0;
    int64_t s64_symbol_index = 0;
    int64_t s64_d = s64_data;

    do {
        s64_temp_value = s64_d;
        s64_d = s64_d / (int64_t)s64_base;
        s64_symbol_index = 15 + (s64_temp_value - (s64_d * (int64_t)s64_base));
        s64_reverse_str[s64_reverse_str_index] = symbols[s64_symbol_index];
        s64_reverse_str_index++;
    } while(s64_d != 0);

    /* Apply negative sign */
    if(s64_temp_value < 0) {
        s64_reverse_str[s64_reverse_str_index] = '-';
        s64_reverse_str_index++;
    }

    s64_result_str_len = s64_reverse_str_index;

    if(s64_len != NULL) {
        *s64_len = s64_result_str_len;
    }

    s64_reverse_str[s64_reverse_str_index] = '\0';
    s64_reverse_str_index--;

    for(s64_result_str_index = 0U; s64_result_str_index < s64_result_str_len; s64_result_str_index++) {
        s64_stringified[s64_result_str_index] = s64_reverse_str[s64_reverse_str_index];
        s64_reverse_str_index--;
    }

    s64_stringified[s64_result_str_index] = '\0';

    return s64_stringified;
}

const char* utoa32_(uint32_t u32_data, char u32_stringified[], uint8_t u32_base, uint32_t* u32_len) {
    char u32_reverse_str[MAX_INT32_STR_LEN_10 + 1U];
    uint32_t u32_reverse_str_index = 0U;
    uint32_t u32_result_str_index = 0U;
    uint32_t u32_result_str_len = 0U;
    uint32_t u32_symbol_index = 0U;
    uint32_t u32_temp_value = 0U;
    uint32_t u32_d = u32_data;

    do {
        u32_temp_value = u32_d;
        u32_d = u32_d / (uint32_t)u32_base;
        u32_symbol_index = 15U + (u32_temp_value - (u32_d * (uint32_t)u32_base));
        u32_reverse_str[u32_reverse_str_index] = symbols[u32_symbol_index];
        u32_reverse_str_index++;
    } while(u32_d != 0U);

    u32_result_str_len = u32_reverse_str_index;

    if(u32_len != NULL) {
        *u32_len = u32_result_str_len;
    }

    u32_reverse_str[u32_reverse_str_index] = '\0';
    u32_reverse_str_index--;

    for(u32_result_str_index = 0U; u32_result_str_index < u32_result_str_len; u32_result_str_index++) {
        u32_stringified[u32_result_str_index] = u32_reverse_str[u32_reverse_str_index];
        u32_reverse_str_index--;
    }

    u32_stringified[u32_result_str_index] = '\0';

    return u32_stringified;
}

const char* utoa64_(uint64_t u64_data, char u64_stringified[], uint8_t u64_base, uint32_t* u64_len) {
    char u64_reverse_str[MAX_INT64_STR_LEN_10 + 1U];
    uint32_t u64_reverse_str_index = 0U;
    uint32_t u64_result_str_index = 0U;
    uint32_t u64_result_str_len = 0U;
    uint64_t u64_symbol_index = 0U;
    uint64_t u64_temp_value = 0U;
    uint64_t u64_d = u64_data;

    do {
        u64_temp_value = u64_d;
        u64_d = u64_d / (uint64_t)u64_base;
        u64_symbol_index = 15U + (u64_temp_value - (u64_d * (uint64_t)u64_base));
        u64_reverse_str[u64_reverse_str_index] = symbols[u64_symbol_index];
        u64_reverse_str_index++;
    } while(u64_d != 0U);

    u64_result_str_len = u64_reverse_str_index;

    if(u64_len != NULL) {
        *u64_len = u64_result_str_len;
    }

    u64_reverse_str[u64_reverse_str_index] = '\0';
    u64_reverse_str_index--;

    for(u64_result_str_index = 0U; u64_result_str_index < u64_result_str_len; u64_result_str_index++) {
        u64_stringified[u64_result_str_index] = u64_reverse_str[u64_reverse_str_index];
        u64_reverse_str_index--;
    }

    u64_stringified[u64_result_str_index] = '\0';

    return u64_stringified;
}

static void dtoa_normal(double_t double_data, int32_t double_precision, char double_stringified[]) {
    uint32_t double_result_str_index = 0U;
    bool double_auto_precision = false;
    char double_number_to_char;
    double_t double_d = double_data;
    int32_t d_precision = double_precision;
    if(double_d < 0.0) {
        double_stringified[double_result_str_index] = '-';
        double_result_str_index++;
        double_d = (-1.0) * double_d;
    }

    if(d_precision < 0) { /* negative precision == automatic precision guess */
        double_auto_precision = true;
        if(double_d < 0.01) {
            d_precision = 10;
        } else if(double_d < 0.1) {
            d_precision = 9;
        } else if(double_d < 1.0) {
            d_precision = 8;
        } else if(double_d < 10.0) {
            d_precision = 7;
        } else if(double_d < 100.0) {
            d_precision = 6;
        } else if(double_d < 1000.0) {
            d_precision = 5;
        } else if(double_d < 10000.0) {
            d_precision = 4;
        } else if(double_d < 100000.0) {
            d_precision = 3;
        } else if(double_d < 1000000.0) {
            d_precision = 2;
        } else if(double_d < 10000000.0) {
            d_precision = 1;
        } else {
            d_precision = 0;
        }
    }

    /* round value according the precision */
    double_d += rounders[d_precision];
    if(d_precision == 0) {
        (void)utoa64_((uint64_t)double_d, &double_stringified[double_result_str_index], 10U, NULL);
    } else {
        uint64_t double_int_part = (uint64_t)double_d;
        uint32_t double_int_part_len = 0U;
        double_d -= (double_t)double_int_part;
        (void)utoa64_(double_int_part, &double_stringified[double_result_str_index], 10U, &double_int_part_len);
        double_result_str_index += double_int_part_len;
        double_stringified[double_result_str_index] = '.';
        double_result_str_index++;

        while(d_precision != 0) {
            d_precision--;
            double_d *= 10.0;
            double_int_part = (uint64_t)double_d;
            double_number_to_char = (char)('0' + double_int_part);
            double_stringified[double_result_str_index] = double_number_to_char;
            double_result_str_index++;
            double_d -= (double_t)double_int_part;
        }

        if(double_auto_precision == true) {
            while(double_stringified[double_result_str_index - 1U] == '0') {
                double_result_str_index--;
            }
            if(double_stringified[double_result_str_index - 1U] == '.') {
                double_result_str_index--;
            }
        }

        double_stringified[double_result_str_index] = '\0';
    }
}

void dtoa_(double_t double_data_, int32_t double_precision_, char out_double_stringified_[]) {
    dtoa_normal(double_data_, double_precision_, out_double_stringified_);
#ifndef DeviceFamily_CC26X2
    switch (__fpclassifyd (double_data_)) {
    case FP_NORMAL:
    case FP_ZERO:
    case FP_SUBNORMAL:
        dtoa_normal (double_data_, double_precision_, out_double_stringified_);
        break;
    case FP_INFINITE:
        (void)strcpy (out_double_stringified_, "INF");
        break;
    case FP_NAN:
        (void)strcpy (out_double_stringified_, "NAN");
        break;
    default:
        (void)strcpy (out_double_stringified_, "???");
        break;
    }
#endif /*DeviceFamily_CC26X2*/
}

static void ftoa_normal(float_t float_data, int32_t float_precision, char float_stringified[]) {
    uint32_t float_result_str_index = 0U;
    bool float_auto_precision = false;
    char float_number_to_char;
    float_t f_data = float_data;
    int32_t f_precision = float_precision;

    if(f_data < 0.0f) {
        float_stringified[float_result_str_index] = '-';
        float_result_str_index++;
        f_data = (-1.0f) * f_data;
    }
    if(f_precision < 0) { /* negative precision == automatic precision guess */
        float_auto_precision = true;
        if(f_data < 0.01) {
            f_precision = 10;
        } else if(f_data < 0.1) {
            f_precision = 9;
        } else if(f_data < 1.0) {
            f_precision = 8;
        } else if(f_data < 10.0) {
            f_precision = 7;
        } else if(f_data < 100.0) {
            f_precision = 6;
        } else if(f_data < 1000.0) {
            f_precision = 5;
        } else if(f_data < 10000.0) {
            f_precision = 4;
        } else if(f_data < 100000.0) {
            f_precision = 3;
        } else if(f_data < 1000000.0) {
            f_precision = 2;
        } else if(f_data < 10000000.0) {
            f_precision = 1;
        } else {
            f_precision = 0;
        }
    }
    /* round value according the precision */
    f_data += rounders[f_precision];
    if(f_precision == 0) {
        (void)utoa64_((uint64_t)f_data, &float_stringified[float_result_str_index], 10U, NULL);
    } else {
        uint64_t float_int_part = (uint64_t)f_data;
        uint32_t float_int_part_len = 0U;
        f_data -= (float_t)float_int_part;
        (void)utoa64_(float_int_part, &float_stringified[float_result_str_index], 10U, &float_int_part_len);
        float_result_str_index += float_int_part_len;
        float_stringified[float_result_str_index] = '.';
        float_result_str_index++;

        while(f_precision != 0) {
            f_precision--;
            f_data *= 10.0f;
            float_int_part = (uint64_t)f_data;
            float_number_to_char = (char)('0' + float_int_part);
            float_stringified[float_result_str_index] = float_number_to_char;
            float_result_str_index++;
            f_data -= (float_t)float_int_part;
        }

        if(float_auto_precision == true) {
            while(float_stringified[float_result_str_index - 1U] == '0') {
                float_result_str_index--;
            }
            if(float_stringified[float_result_str_index - 1U] == '.') {
                float_result_str_index--;
            }
        }

        float_stringified[float_result_str_index] = '\0';
    }
}

// uint8_t __fpclassifyf(float_t float_data_) { return FP_SUBNORMAL; }

void ftoa_(float_t float_data_, int32_t float_precision_, char float_stringified_[]) {
    switch(__fpclassifyf(float_data_)) {
    case FP_NORMAL:
    case FP_ZERO:
    case FP_SUBNORMAL:
        ftoa_normal(float_data_, float_precision_, float_stringified_);
        break;
    case FP_INFINITE:
        (void)strcpy(float_stringified_, "INF");
        break;
    case FP_NAN:
        (void)strcpy(float_stringified_, "NAN");
        break;
    default:
        (void)strcpy(float_stringified_, "???");
        break;
    }
}

uint32_t base64_encode(const uint8_t encode_data[], uint32_t encode_data_size, char encode_result[]) {
    static char basis64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    uint32_t encode_result_index = 0U;
    uint32_t encode_data_base_index = 0U;
    uint8_t basis64_char_index;
    uint8_t encode_data_char_0;
    uint8_t encode_data_char_1;
    uint8_t encode_data_char_2;
    uint32_t s = encode_data_size;

    while(s > 2U) {
        encode_data_char_0 = encode_data[0U + encode_data_base_index];
        encode_data_char_1 = encode_data[1U + encode_data_base_index];
        encode_data_char_2 = encode_data[2U + encode_data_base_index];

        basis64_char_index = (encode_data_char_0 >> 2U) & 0x3fU;
        encode_result[encode_result_index] = basis64[basis64_char_index];
        encode_result_index++;

        basis64_char_index = (uint8_t)((encode_data_char_0 & 3U) << 4U) | (encode_data_char_1 >> 4U);
        encode_result[encode_result_index] = basis64[basis64_char_index];
        encode_result_index++;

        basis64_char_index = (uint8_t)((encode_data_char_1 & 0x0fU) << 2U) | (encode_data_char_2 >> 6U);
        encode_result[encode_result_index] = basis64[basis64_char_index];
        encode_result_index++;

        basis64_char_index = encode_data_char_2 & 0x3fU;
        encode_result[encode_result_index] = basis64[basis64_char_index];
        encode_result_index++;

        encode_data_base_index += 3U;
        s -= 3U;
    }

    if(s != 0U) {
        encode_data_char_0 = encode_data[0U + encode_data_base_index];
        encode_data_char_1 = encode_data[1U + encode_data_base_index];

        basis64_char_index = (encode_data_char_0 >> 2U) & 0x3fU;
        encode_result[encode_result_index] = basis64[basis64_char_index];
        encode_result_index++;

        if(s == 1U) {
            basis64_char_index = (uint8_t)((encode_data_char_0 & 3U) << 4U);
            encode_result[encode_result_index] = basis64[basis64_char_index];
            encode_result_index++;
            encode_result[encode_result_index] = '=';
            encode_result_index++;
        } else {
            basis64_char_index = (uint8_t)((encode_data_char_0 & 3U) << 4U) | (encode_data_char_1 >> 4U);
            encode_result[encode_result_index] = basis64[basis64_char_index];
            encode_result_index++;

            basis64_char_index = (uint8_t)((encode_data_char_1 & 0x0fU) << 2U);
            encode_result[encode_result_index] = basis64[basis64_char_index];
            encode_result_index++;
        }

        encode_result[encode_result_index] = '=';
        encode_result_index++;
    }

    encode_result[encode_result_index] = '\0';
    return encode_result_index;
}

#define XX 222U
#define BASE64_EQ 61U

bool base64_decode(const char decode_str[], int32_t decode_str_len, uint8_t decode_data[], uint32_t* decode_data_size) {
    static const uint8_t basis[] = {
        XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,
        XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  62U,
        XX,  XX,  XX,  63U, 52U, 53U, 54U, 55U, 56U, 57U, 58U, 59U, 60U, 61U, XX,  XX,  XX,  XX,  XX,  XX,  XX,  0U,
        1U,  2U,  3U,  4U,  5U,  6U,  7U,  8U,  9U,  10U, 11U, 12U, 13U, 14U, 15U, 16U, 17U, 18U, 19U, 20U, 21U, 22U,
        23U, 24U, 25U, XX,  XX,  XX,  XX,  XX,  XX,  26U, 27U, 28U, 29U, 30U, 31U, 32U, 33U, 34U, 35U, 36U, 37U, 38U,
        39U, 40U, 41U, 42U, 43U, 44U, 45U, 46U, 47U, 48U, 49U, 50U, 51U, XX,  XX,  XX,  XX,  XX,

        XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,
        XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,
        XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,
        XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,
        XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,
        XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX,  XX};

    bool decode_success = true;
    uint32_t decode_data_index = 0U;
    uint32_t decode_str_base_index = 0U;
    int32_t i;
    uint8_t decode_value;
    uint8_t decode_str_char_0;
    uint8_t decode_str_char_1;
    uint8_t decode_str_char_2;
    uint8_t decode_str_char_3;
    int32_t len = decode_str_len;

    *decode_data_size = 0U;
    if(len < 0) {
        len = (int32_t)strlen(decode_str);
    }

    if((len % 4) != 0) {
        decode_success = false;
    }

    for(i = 0; i < len; i++) {
        uint8_t decode_str_char = (uint8_t)decode_str[i];
        if((decode_success == false) || (decode_str_char == BASE64_EQ)) {
            break;
        }
        if(basis[decode_str_char] == XX) {
            decode_success = false;
        }
    }

    if(decode_success == true) {
        while(len > 4) {
            decode_str_char_0 = (uint8_t)decode_str[0U + decode_str_base_index];
            decode_str_char_1 = (uint8_t)decode_str[1U + decode_str_base_index];
            decode_str_char_2 = (uint8_t)decode_str[2U + decode_str_base_index];
            decode_str_char_3 = (uint8_t)decode_str[3U + decode_str_base_index];

            decode_value = ((uint8_t)(basis[decode_str_char_0] << 2U) | (uint8_t)(basis[decode_str_char_1] >> 4U));
            decode_data[decode_data_index] = decode_value;
            decode_data_index++;

            decode_value = ((uint8_t)(basis[decode_str_char_1] << 4U) | (uint8_t)(basis[decode_str_char_2] >> 2U));
            decode_data[decode_data_index] = decode_value;
            decode_data_index++;

            decode_value = ((uint8_t)(basis[decode_str_char_2] << 6U) | basis[decode_str_char_3]);
            decode_data[decode_data_index] = decode_value;
            decode_data_index++;

            decode_str_base_index += 4U;
            len -= 4;
        }

        if(len != 0) {
            decode_str_char_0 = (uint8_t)decode_str[0U + decode_str_base_index];
            decode_str_char_1 = (uint8_t)decode_str[1U + decode_str_base_index];
            decode_str_char_2 = (uint8_t)decode_str[2U + decode_str_base_index];
            decode_str_char_3 = (uint8_t)decode_str[3U + decode_str_base_index];

            decode_value = ((uint8_t)(basis[decode_str_char_0] << 2U) | (uint8_t)(basis[decode_str_char_1] >> 4U));
            decode_data[decode_data_index] = decode_value;
            decode_data_index++;
            if(decode_str_char_2 != BASE64_EQ) {
                decode_value = ((uint8_t)(basis[decode_str_char_1] << 4U) | (uint8_t)(basis[decode_str_char_2] >> 2U));
                decode_data[decode_data_index] = decode_value;
                decode_data_index++;
                if(decode_str_char_3 != BASE64_EQ) {
                    decode_value = ((uint8_t)(basis[decode_str_char_2] << 6U) | basis[decode_str_char_3]);
                    decode_data[decode_data_index] = decode_value;
                    decode_data_index++;
                }
            }
        }

        *decode_data_size = decode_data_index;
    }

    return decode_success;
}

const char* rx_ltoa32(int32_t ltoa32_data) {
    static char ltoa32_str[(MAX_INT32_STR_LEN_10 + 1U)];
    return ltoa32_(ltoa32_data, ltoa32_str, 10U, NULL);
}

const char* rx_ltoa64(int64_t ltoa64_data) {
    static char ltoa64_str[(MAX_INT64_STR_LEN_10 + 1U)];
    return ltoa64_(ltoa64_data, ltoa64_str, 10U, NULL);
}

const char* rx_utoa32(uint32_t utoa32_data) {
    static char utoa32_str[(MAX_INT32_STR_LEN_10 + 1U)];
    return utoa32_(utoa32_data, utoa32_str, 10U, NULL);
}

const char* rx_utoa64(uint64_t utoa64_data) {
    static char utoa64_str[(MAX_INT64_STR_LEN_10 + 1U)];
    return utoa64_(utoa64_data, utoa64_str, 10U, NULL);
}

const char* utoa_hex32(uint32_t u32_hex_data) {
    static char utoa_hex32_str[(MAX_INT32_STR_LEN_16 + 1U)];
    return utoa32_(u32_hex_data, utoa_hex32_str, 16U, NULL);
}

const char* utoa_hex64(uint64_t u64_hex_data) {
    static char utoa_hex64_str[(MAX_INT64_STR_LEN_16 + 1U)];
    return utoa64_(u64_hex_data, utoa_hex64_str, 16U, NULL);
}

const char* ltoa_hex32(int32_t s32_hex_data) {
    static char ltoa_hex32_str[(MAX_INT32_STR_LEN_16 + 1U)];
    return ltoa32_(s32_hex_data, ltoa_hex32_str, 16U, NULL);
}

const char* ltoa_hex64(int64_t s64_hex_data) {
    static char ltoa_hex64_str[(MAX_INT64_STR_LEN_16 + 1U)];
    return ltoa64_(s64_hex_data, ltoa_hex64_str, 16U, NULL);
}

const char* rx_dtoa(double_t d) {
    static char dtoa_str[(((MAX_INT64_STR_LEN_10 + MAX_PRECISION) + 1U) + 1U)];
    dtoa_(d, -1, dtoa_str);
    return dtoa_str;
}

const char* rx_ftoa(float_t float_v) {
    static char ftoa_str[(((MAX_INT64_STR_LEN_10 + MAX_PRECISION) + 1U) + 1U)];
    ftoa_(float_v, -1, ftoa_str);
    return ftoa_str;
}

static bool try_hex_char_to_u8(uint8_t hex_char, uint8_t* hex_char_to_u8_value) {
    uint8_t hex_char_to_u8_result = 0U;
    bool hex_char_to_u8_success = true;

    switch(hex_char) {
    case(uint8_t)'0':
    case(uint8_t)'1':
    case(uint8_t)'2':
    case(uint8_t)'3':
    case(uint8_t)'4':
    case(uint8_t)'5':
    case(uint8_t)'6':
    case(uint8_t)'7':
    case(uint8_t)'8':
    case(uint8_t)'9':
        hex_char_to_u8_result = hex_char - (uint8_t)'0';
        break;
    case(uint8_t)'a':
    case(uint8_t)'b':
    case(uint8_t)'c':
    case(uint8_t)'d':
    case(uint8_t)'e':
    case(uint8_t)'f':
        hex_char_to_u8_result = ((hex_char - (uint8_t)'a') + 10U);
        break;
    case(uint8_t)'A':
    case(uint8_t)'B':
    case(uint8_t)'C':
    case(uint8_t)'D':
    case(uint8_t)'E':
    case(uint8_t)'F':
        hex_char_to_u8_result = ((hex_char - (uint8_t)'A') + 10U);
        break;
    default:
        hex_char_to_u8_success = false;
        break;
    }
    if(NULL != hex_char_to_u8_value) {
        (*hex_char_to_u8_value) = hex_char_to_u8_result;
    }
    return hex_char_to_u8_success;
}

static bool try_dec_char_to_u8(uint8_t dec_char, uint8_t* dec_char_to_u8_value) {
    uint8_t dec_char_to_u8_result = 0U;
    bool dec_char_to_u8_success = true;

    if((dec_char < (uint8_t)'0') || (dec_char > (uint8_t)'9')) {
        dec_char_to_u8_success = false;
    }

    if(dec_char_to_u8_success == true) {
        dec_char_to_u8_result = dec_char - (uint8_t)'0';
    }
    if(NULL != dec_char_to_u8_value) {
        (*dec_char_to_u8_value) = dec_char_to_u8_result;
    }
    return dec_char_to_u8_success;
}

static bool is_signed(const char first_str_char, int32_t* first_digit_index) {
    bool negative = false;
    *first_digit_index = 0;
    if(first_str_char == '-') {
        negative = true;
        *first_digit_index = 1;
    }
    if(first_str_char == '+') {
        *first_digit_index = 1;
    }
    return negative;
}

static bool get_str_len(const char char_str[], int32_t* str_len) {
    bool str_not_empty = true;
    if(NULL != char_str) {
        if(*str_len < 0) {
            if(char_str != NULL) {
                *str_len = (int32_t)strlen(char_str);
            } else {
                *str_len = 0;
            }
        }

        if(*str_len == 0) {
            str_not_empty = false;
        }
    } else {
        str_not_empty = false;
    }

    return str_not_empty;
}

static bool is_dec_digit(const char character) {
    bool res = false;
    if(('0' <= character) && (character <= '9')) {
        res = true;
    } else {
        res = false;
    }
    return res;
}

bool is_dec_str(const char str_to_check[], int32_t str_to_check_len) {
    bool is_dec_str_result = false;
    int32_t valid_dec_cnd = 0;
    if(NULL != str_to_check) {
        bool len_check_passed = (0 < str_to_check_len);
        if(true == len_check_passed) {
            int32_t i = 0;
            for(i = 0; i < str_to_check_len; i++) {
                if(true == is_dec_digit(str_to_check[i])) {
                    valid_dec_cnd++;
                }
            }
            if(str_to_check_len == valid_dec_cnd) {
                is_dec_str_result = true;
            }
        }
    }
    return is_dec_str_result;
}

bool is_hex_str(const char str_to_check[], int32_t str_to_check_len, uint8_t* const out_shift) {
    bool is_hex_str_result = false;
    int32_t validHexCnt = 0;
    uint8_t out_shift_loc = 0U;
    if((NULL != str_to_check) && (NULL != out_shift)) {
        bool is_dec_str_res = is_dec_str(str_to_check, str_to_check_len);
        if(false == is_dec_str_res) {
            bool len_check_passed = (str_to_check_len > 2);
            if(true == len_check_passed) {
                if(((char)'0' == str_to_check[0]) &&
                   (((char)'x' == str_to_check[1]) || ((char)'X' == str_to_check[1]))) {
                    out_shift_loc = 2U;
                } else {
                    out_shift_loc = 0U;
                }

                int32_t i = 0;
                for(i = ((int32_t)out_shift_loc); i < str_to_check_len; i++) {
                    if(true == is_hex_digit(str_to_check[i])) {
                        validHexCnt++;
                    }
                }
                if((str_to_check_len - ((int32_t)out_shift_loc)) == validHexCnt) {
                    (*out_shift) = out_shift_loc;
                    is_hex_str_result = true;
                }
            }
        }
    }
    return is_hex_str_result;
}

static bool is_hex_digit(const char character) {
    bool res = false;
    if((('A' <= character) && (character <= 'F')) || (('a' <= character) && (character <= 'f')) ||
       (('0' <= character) && (character <= '9'))) {
        res = true;
    } else {
        res = false;
    }
    return res;
}

static bool rx_strtod(const char str[], const char** endptr, double_t* result) {
    bool strtod_success = true;
    if(NULL != result) {
        bool strtod_negative = false;
        double_t number = 0.0;
        uint32_t u32_number;
        uint32_t str_index = 0U;
        int32_t s32_number;
        int32_t exponent = 0;
        uint8_t num_digits = 0U;
        uint8_t num_decimals = 0U;
        uint8_t temp_value = 0U;

        /* Skip leading whitespace */
        while(isspace((int32_t)(str[str_index])) > 0) {
            str_index++;
        }

        /* Handle optional sign */
        if(('-' == str[str_index]) || ('+' == str[str_index])) {
            if('-' == str[str_index]) {
                strtod_negative = true;
            }
            str_index++;
        }

        /* Process string of digits */
        while(isdigit((int32_t)(str[str_index])) > 0) {
            (void)try_dec_char_to_u8((uint8_t)str[str_index], &temp_value);
            number = (number * 10.0) + (double_t)temp_value;
            str_index++;
            num_digits++;
        }

        /* Process decimal part  */
        if(str[str_index] == '.') {
            str_index++;

            while(isdigit((int32_t)(str[str_index])) > 0) {
                (void)try_dec_char_to_u8((uint8_t)str[str_index], &temp_value);
                number = (number * 10.0) + (double_t)temp_value;
                str_index++;
                num_digits++;
                num_decimals++;
            }

            exponent -= (int32_t)num_decimals;
        }

        if(num_digits == 0U) {
            if(endptr != NULL) {
                *endptr = &str[str_index];
            }

            strtod_success = false;
        }
        if(strtod_success == true) {
            /* Correct for sign */
            if(strtod_negative == true) {
                number = (-1.0) * number;
            }

            /* Process an exponent string */
            if((str[str_index] == 'e') || (str[str_index] == 'E')) {
                /* Handle optional sign */
                strtod_negative = false;
                str_index++;

                if(('-' == str[str_index]) || ('+' == str[str_index])) {
                    if('-' == str[str_index]) {
                        strtod_negative = true;
                    }
                    str_index++;
                }

                if(isdigit((int32_t)(str[str_index])) > 0) {
                    /* Process string of digits */
                    s32_number = 0;
                    while(isdigit((int32_t)(str[str_index])) > 0) {
                        (void)try_dec_char_to_u8((uint8_t)str[str_index], &temp_value);
                        s32_number = (s32_number * 10) + (int32_t)temp_value;
                        str_index++;
                    }

                    if(strtod_negative == true) {
                        exponent -= s32_number;
                    } else {
                        exponent += s32_number;
                    }
                } else { /* No exponent after e */
                    strtod_success = false;
                }
            }

            if((exponent < DBL_MIN_10_EXP) || (exponent > DBL_MAX_10_EXP)) {
                if(endptr != NULL) {
                    *endptr = &str[str_index];
                }

                strtod_success = false;
            }

            if(strtod_success == true) {
                /* Scale the result */
                double_t p10 = 10.0;
                s32_number = exponent;
                if(s32_number < 0) {
                    s32_number = -s32_number;
                }

                u32_number = (uint32_t)(s32_number);

                while(u32_number != 0U) {
                    if((u32_number & 1U) > 0U) {
                        if(exponent < 0) {
                            number /= p10;
                        } else {
                            number *= p10;
                        }
                    }
                    u32_number >>= 1;
                    p10 *= p10;
                }

                if(endptr != NULL) {
                    *endptr = &str[str_index];
                }
                if(NULL != result) {
                    *result = number;
                }
            }
        }
    } else {
        strtod_success = false;
    }
    return strtod_success;
}

static bool is_true(const char* true_str_to_check) {
    bool is_true_result = false;
    if(NULL != true_str_to_check) {
        int32_t check_for_yes = strcmp(true_str_to_check, "yes");
        int32_t check_for_true = strcmp(true_str_to_check, "true");
        int32_t check_for_on = strcmp(true_str_to_check, "on");
        int32_t check_for_1 = strcmp(true_str_to_check, "1");

        if((check_for_yes == 0) || (check_for_true == 0) || (check_for_on == 0) || (check_for_1 == 0)) {
            is_true_result = true;
        }
    }

    return is_true_result;
}

static bool is_false(const char* false_str_to_check) {
    bool is_false_result = false;
    if(NULL != false_str_to_check) {
        int32_t check_for_no = strcmp(false_str_to_check, "no");
        int32_t check_for_false = strcmp(false_str_to_check, "false");
        int32_t check_for_off = strcmp(false_str_to_check, "off");
        int32_t check_for_0 = strcmp(false_str_to_check, "0");

        if((check_for_no == 0) || (check_for_false == 0) || (check_for_off == 0) || (check_for_0 == 0)) {
            is_false_result = true;
        }
    }

    return is_false_result;
}

const char* utoa_bin8(uint8_t u8_bin_data) {
    uint8_t cell8 = 0u;
    uint8_t mask8 = 0x80U;
    static char outBitStr8[sizeof("0000_0000") + 1U] = "0000_0000";
    while(mask8 != 0U) {
        if(outBitStr8[cell8] == '_') {
            cell8++;
        }
        if(0u != (u8_bin_data & mask8)) {
            outBitStr8[cell8] = '1';
        } else {
            outBitStr8[cell8] = '0';
        }
        mask8 >>= 1U;
        cell8++;
    }
    outBitStr8[sizeof(outBitStr8) - 1u] = '\0';
    return outBitStr8;
}

const char* utoa_bin16(uint16_t u16_bin_data) {
    uint8_t cell16 = 0u;
    uint16_t mask16 = 0x8000U;
    static char outBitStr16[sizeof("0000_0000_0000_0000") + 1U] = "0000_0000_0000_0000";
    while(mask16 != 0U) {
        if(outBitStr16[cell16] == '_') {
            cell16++;
        }
        if(0u != (u16_bin_data & mask16)) {
            outBitStr16[cell16] = '1';
        } else {
            outBitStr16[cell16] = '0';
        }
        mask16 >>= 1U;
        cell16++;
    }
    outBitStr16[sizeof(outBitStr16) - 1u] = '\0';
    return outBitStr16;
}

const char* utoa_bin24(uint32_t u32_bin_data) {
    uint8_t cell24 = 0u;
    uint32_t mask24 = 0x00800000U;
    static char outBitStr24[sizeof("0000_0000_0000_0000_0000_0000") + 1U] = "0000_0000_0000_0000_0000_0000";
    while(mask24 != 0U) {
        if(outBitStr24[cell24] == '_') {
            cell24++;
        }
        if(0u != (u32_bin_data & mask24)) {
            outBitStr24[cell24] = '1';
        } else {
            outBitStr24[cell24] = '0';
        }
        mask24 >>= 1U;
        cell24++;
    }
    outBitStr24[sizeof(outBitStr24) - 1u] = '\0';
    return outBitStr24;
}

const char* utoa_bin32(uint32_t u32_bin_data) {
    uint8_t cell32 = 0u;
    uint32_t mask32 = 0x80000000U;
    static char outBitStr32[sizeof("0000_0000_0000_0000_0000_0000_0000_0000") + 1U] =
        "0000_0000_0000_0000_0000_0000_0000_0000";
    while(mask32 != 0U) {
        if(outBitStr32[cell32] == '_') {
            cell32++;
        }
        if(0u != (u32_bin_data & mask32)) {
            outBitStr32[cell32] = '1';
        } else {
            outBitStr32[cell32] = '0';
        }
        mask32 >>= 1U;
        cell32++;
    }
    outBitStr32[sizeof(outBitStr32) - 1u] = '\0';
    return outBitStr32;
}

const char* bool2name(bool val) {
    const char* name = "undef";
    if(true == val) {
        name = "on";
    }
    if(false == val) {
        name = "off";
    }
    return name;
}

const char* bool2test_status(bool val) {
    const char* name = "undef";
    if(true == val) {
        name = "+";
    }
    if(false == val) {
        name = "fail";
    }
    return name;
}


/*Mind byte order of you CPU core*/
uint32_t assemble_uint32(uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4) {
    uint32_t v32 = 0;
    uint8_t v8[4] = {0x00, 0x00, 0x00, 0x00};
    v8[0] = byte4;
    v8[1] = byte3;
    v8[2] = byte2;
    v8[3] = byte1;
    (void)memcpy((uint8_t*)&v32, (uint8_t*)&v8[0], 4);

    return v32;
}

uint16_t float_to_uint16(float val) {
    uint16_t out = (uint16_t)val;
    return out;
}

bool try_str2array(char* in_str_array, uint8_t* out_array, uint16_t array_size, uint16_t* out_array_len) {
    bool res = false;
    uint8_t out_shift = 0;
    uint32_t len_str_in = strlen(in_str_array);
    res = is_hex_str(in_str_array, len_str_in, &out_shift);
    if(true == res) {
        if(0 == (len_str_in % 2)) {
            uint32_t i;
            for(i = 0; i < (len_str_in / 2); i++) {
                res = try_strl2uint8_hex(&in_str_array[out_shift + i * 2], 2, &out_array[i]);
                if(true == res) {
                    (*out_array_len)++;
                } else {
                    break;
                }
            }
            if(0U < (*out_array_len)) {
                res = true;
            }
        }
    }

    return res;
}
