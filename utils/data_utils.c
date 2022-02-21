#include "data_utils.h"

#include <math.h>
#include <string.h>

#include "array.h"

uint8_t uint16_to_uint8_limiter(uint16_t in_val) {
    uint8_t out_val = 0xFF;
    if(in_val < 0xFF) {
        out_val = in_val;
    }
    return out_val;
}

uint32_t ceil4byte(uint32_t val) {
    uint32_t out_val = val;
    while((out_val & 3) != 0) {
        out_val++;
    }
    return out_val;
}

uint8_t uint8_limiter(uint8_t in_val, uint8_t up_val) {
    uint8_t out_val = up_val;
    if(in_val <= up_val) {
        out_val = in_val;
    }
    return out_val;
}

uint8_t max8u(uint8_t max8u_x1, uint8_t max8u_x2) { return (((max8u_x1) > (max8u_x2)) ? (max8u_x1) : (max8u_x2)); }

uint8_t rx_min8u(uint8_t min8u_x1, uint8_t min8u_x2) { return (((min8u_x1) < (min8u_x2)) ? (min8u_x1) : (min8u_x2)); }

uint16_t max16u(uint16_t max16u_x1, uint16_t max16u_x2) {
    return (((max16u_x1) > (max16u_x2)) ? (max16u_x1) : (max16u_x2));
}

uint16_t min16u(uint16_t min16u_x1, uint16_t min16u_x2) {
    return (((min16u_x1) < (min16u_x2)) ? (min16u_x1) : (min16u_x2));
}

uint32_t max32u(uint32_t max32u_x1, uint32_t max32u_x2) {
    return (((max32u_x1) > (max32u_x2)) ? (max32u_x1) : (max32u_x2));
}

uint64_t max64u(uint64_t max64u_x1, uint64_t max64u_x2) {
    return (((max64u_x1) > (max64u_x2)) ? (max64u_x1) : (max64u_x2));
}

uint32_t min32u(uint32_t min32u_x1, uint32_t min32u_x2) {
    return (((min32u_x1) < (min32u_x2)) ? (min32u_x1) : (min32u_x2));
}

uint64_t min64u(uint64_t min64u_x1, uint64_t min64u_x2) {
    return (((min64u_x1) < (min64u_x2)) ? (min64u_x1) : (min64u_x2));
}

int8_t rx_max8(int8_t max8_x1, int8_t max8_x2) { return (((max8_x1) > (max8_x2)) ? (max8_x1) : (max8_x2)); }

int8_t rx_min8(int8_t min8_x1, int8_t min8_x2) { return (((min8_x1) < (min8_x2)) ? (min8_x1) : (min8_x2)); }

int16_t max16(int16_t max16_x1, int16_t max16_x2) { return (((max16_x1) > (max16_x2)) ? (max16_x1) : (max16_x2)); }

int16_t rx_min16(int16_t min16_x1, int16_t min16_x2) { return (((min16_x1) < (min16_x2)) ? (min16_x1) : (min16_x2)); }

int32_t rx_max32(int32_t max32_x1, int32_t max32_x2) { return (((max32_x1) > (max32_x2)) ? (max32_x1) : (max32_x2)); }

int32_t rx_min32(int32_t min32_x1, int32_t min32_x2) { return (((min32_x1) < (min32_x2)) ? (min32_x1) : (min32_x2)); }

uint32_t swap32(uint32_t u32_num) {
    return ((((u32_num) >> 24) & 0xffU) | (((u32_num) << 8) & 0xff0000U) | (((u32_num) >> 8) & 0xff00U) |
            (((u32_num) << 24) & 0xff000000U));
}

uint16_t swap16(uint16_t u16_num) {
    return (((uint16_t)((u16_num) >> 8) & 0xffU) | ((uint16_t)((u16_num) << 8) & 0xff00U));
}

bool swap8(uint8_t* u8_x, uint8_t* u8_y) {
    bool res = false;
    if(u8_x && u8_y) {
        uint8_t temp = *u8_x;
        *u8_x = *u8_y;
        *u8_y = temp;
        res = true;
    }
    return res;
}

#define TEN_SPACES "          "
const char* get_spaces_str(uint32_t spaces_length) {
    static char szSpaces[101] =
        TEN_SPACES TEN_SPACES TEN_SPACES TEN_SPACES TEN_SPACES TEN_SPACES TEN_SPACES TEN_SPACES TEN_SPACES TEN_SPACES;
    uint32_t last_index = sizeof(szSpaces) - 1U;
    if(spaces_length > last_index) {
        last_index = 0U;
    } else {
        last_index -= spaces_length;
    }
    return &szSpaces[last_index];
}

int8_t two_complement_to_decimal(uint8_t in_code, int significant_bits) {
    int power = (int)pow((double)2.0f, (double)(significant_bits - 1));
    int sum = 0;
    int i = 0;
    for(i = significant_bits - 1; 0 <= i; i--) {
        if((i == (significant_bits - 1)) && ((1 << i) == ((1 << i) & in_code))) {
            sum = power * -1;
        } else {
            if(in_code & (1 << i)) {
                sum += power;
            }
        }
        power /= 2;
    }

    return sum;
}

float ms2Hz(uint16_t ms) {
    float hz = (1.0f * 1000.0f) / ((float)ms);
    return hz;
}

uint8_t extract_digit(uint32_t in_num, uint8_t digit_index) {
    uint8_t i = 0;
    uint8_t digit = 0;
    uint8_t out_digit = 0;
    uint32_t num = in_num;
    while(0 < num) {
        digit = num % 10;
        if(digit_index == i) {
            out_digit = digit;
            break;
        }
        num = num / 10;
        i++;
    }
    return out_digit;
}

#if 0
bool try_alloc_on_stack(int n, uint8_t pat, uint16_t* real_size) {
    // uint8_t array[n];
    bool res = false;
    uint8_t array[n]; // actually on heap
    *real_size = sizeof(array);

    memset(array, pat, n);
    if(is_arr_pat(array, n, pat)) {
        res = true;
    }

    return res;
}
#endif

const static char hex2asciiLUT[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
bool hex2ascii(uint8_t* in_hex, uint32_t hex_len, uint8_t* out_ascii, uint32_t ascii_len) {
    bool res = false;
    if(in_hex && out_ascii && ((2 * hex_len + 1) <= ascii_len) && (0 < hex_len)) {
        res = true;
        uint32_t i = 0;
        uint32_t j = 0;
        for(i = 0; i < hex_len; i++) {
            out_ascii[j++] = hex2asciiLUT[MASK_4BIT & (in_hex[i] >> 4)];
            out_ascii[j++] = hex2asciiLUT[MASK_4BIT & (in_hex[i])];
        }
        out_ascii[j] = 0x00;
    }
    return res;
}

uint32_t ipow(uint32_t base, uint32_t exponenta) {
    uint32_t ret = 1, i = 0;
    if(0 != exponenta) {
        for(i = 1; i <= exponenta; i++) {
            ret *= base;
        }
    }
    return ret;
}

bool mem_scan(uint8_t* base, uint32_t size, float* usage_pec, uint32_t* spare, uint32_t* busy) {
    bool res = false;
    if(usage_pec && spare && busy) {
        res = true;
        *spare = 0;
        *busy = 0;
        uint8_t* addr = base;
        uint32_t cnt = 0;
        for(addr = base, cnt = 0; addr < (base + size); addr++, cnt++) {
            if(0xFF == (*addr)) {
                (*spare)++;
            } else {
                (*busy)++;
            }
            if(!(cnt % 1000)) {
                // wait_in_loop_ms(5);
            }
        }
        *usage_pec = (float)(((float)(100U * (*busy))) / ((float)size));
    }
    return res;
}
