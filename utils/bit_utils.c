#include "bit_utils.h"

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "io_utils.h"

/*3 -> 0b0111=7*/
uint32_t generate_32bit_left_mask(uint8_t bit_len) {
    uint32_t mask = 0x00000000U;
    if(bit_len <= 32) {
        uint32_t i = 0U;
        for(i = 0U; i < bit_len; i++) {
            mask |= (1 << i);
        }
    }
    return mask;
}

uint32_t generate_32bit_custom_mask(uint8_t max_bit, uint8_t min_bit) {
    uint32_t mask = 0x00000000U;
    if(min_bit <= max_bit) {
        uint32_t i = 0U;
        for(i = min_bit; i <= max_bit; i++) {
            mask |= (1 << i);
        }
    }
    return mask;
}

uint32_t extract_subval_from_32bit(uint32_t in_val, uint8_t max_bit, uint8_t min_bit) {
    uint32_t outVal = 0;
    if((min_bit <= max_bit) && (max_bit <= 31) && (min_bit <= 31)) {
        uint32_t mask = generate_32bit_left_mask(max_bit - min_bit + 1);
        outVal = (in_val >> min_bit);
        outVal = outVal & mask;
    }
    return outVal;
}

uint32_t insert_subval_in_32bit(uint32_t orig_val, uint32_t sub_val, uint8_t max_bit, uint8_t min_bit) {
    uint32_t out_val = 0;
    uint32_t temp_val = orig_val;
    uint32_t mask = 0;
    mask = generate_32bit_custom_mask(max_bit, min_bit);
    temp_val = temp_val >> min_bit;
    temp_val |= sub_val;
    temp_val = temp_val << min_bit;
    temp_val = temp_val | (mask & orig_val);
    /*verify*/
    out_val = temp_val;
    uint32_t read_sub_val = extract_subval_from_32bit(out_val, max_bit, min_bit);
    if(read_sub_val != sub_val) {
        out_val = 0xFFFFFFFF;
    }
    return out_val;
}

uint64_t generate_64bit_left_mask(uint8_t bit_len) {
    uint64_t mask = 0x00000000U;
    if(bit_len <= 64) {
        uint32_t i = 0U;
        for(i = 0U; i < bit_len; i++) {
            mask |= (1 << i);
        }
    }
    return mask;
}

uint64_t extract_subval_from_64bit(uint64_t in_val, uint8_t max_bit, uint8_t min_bit) {
    uint64_t outVal = 0;
    if((min_bit <= max_bit) && (max_bit <= 63) && (min_bit <= 63)) {
        uint64_t mask = generate_64bit_left_mask(max_bit - min_bit + 1);
        outVal = (in_val >> min_bit);
        outVal = outVal & mask;
    }
    return outVal;
}

bool bit32_control_proc(uint32_t* p_reg, char cmd, uint8_t bit_num) {
    bool res = false;
    if(bit_num <= 31) {
        res = true;
        // uint32_t volatile * const p_reg = (uint32_t *) address_val;
        io_printf("%p %u " CRLF, p_reg, p_reg[0]);
        // uint32_t* val = (uint32_t*)address_val;
        switch(cmd) {
        case 's':
            p_reg[0] = p_reg[0] | (1U << (bit_num));
            break;
        case 'r':
            p_reg[0] = p_reg[0] & (~(1U << (bit_num)));
            break;
        case 't':
            p_reg[0] = p_reg[0] ^ (1U << (bit_num));
            break;
        default:
            res = false;
            break;
        }
        io_printf("%p %u " CRLF, p_reg, p_reg[0]);
    }
    return res;
}
