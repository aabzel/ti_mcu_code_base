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

uint8_t generate_8bit_left_mask(uint8_t bit_len) {
    uint8_t mask = 0x00000000U;
    if(bit_len <= 8) {
        uint8_t i = 0U;
        for(i = 0U; i < bit_len; i++) {
            mask |= (1 << i);
        }
    }
    return mask;
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
        // io_printf("%p %u " CRLF, p_reg, p_reg[0]);
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
        // io_printf("%p %u " CRLF, p_reg, p_reg[0]);
    }
    return res;
}

uint8_t extract_subval_from_8bit(uint8_t in_val, uint8_t max_bit, uint8_t min_bit) {
    uint64_t outVal = 0;
    if((min_bit <= max_bit) && (max_bit <= 7) && (min_bit <= 7)) {
        uint64_t mask = generate_8bit_left_mask(max_bit - min_bit + 1);
        outVal = (in_val >> min_bit);
        outVal = outVal & mask;
    }
    return outVal;
}

/*
 * Function returns the number of set bits in binary  (Hamming weight)
 * representation of positive integer val
 * TODO: we can boost time of calculation by creating look up table for byte->am of bit
 *       It will reqire 512 byte of flash for 8bit value.
 *
 * */

uint8_t count_set_bits(uint32_t const inVal32bit) {
    uint8_t sum = 0;
    uint32_t val32bit = inVal32bit;
    while(0u != val32bit) {
        sum++;
        val32bit &= (val32bit - 1u);
    }
    return sum;
}

/*
 *  Odd (Amount of 1 bits is 1 3 5 7 9 and so 31)
 *  even (Amount of 1 bits is 2 4 6 8 10 and so 32)
 *
 *  returns 1 if the parity in inValue is Odd
 *  (Amount of 1 bits is 1 3 5 7 9 and so 31)
 *
 *  inVal        bit        setBit  parity        return
 *  0          > 00       > 0  >    PARITY_EVEN   false
 *  1          > 01       > 1  >    PARITY_ODD    true
 *  2          > 10       > 1  >    PARITY_ODD    true
 *  3          > 11       > 2  >    PARITY_EVEN   false
 *  178        > 10110010 > 4  >    PARITY_EVEN   false
 *  0xFFFFFFFF > 1..111   > 32 >    PARITY_EVEN   false
 *  0xFFFFFFFE > 11..10   > 31 >    PARITY_ODD    true
 */
bool is_parity_odd(uint32_t const inVal) {
    bool otRet = false;
    uint8_t cntOnes = 0u;
    /*count set bits*/
    cntOnes = count_set_bits(inVal);
    if(0u < (cntOnes & 1u)) {
        otRet = true; /*(Amount of 1 bits is 1 3 5 7 9 and so 31)*/
    } else {
        otRet = false; /*(Amount of 1 bits is 2 4 6 8 10 and so 32)*/
    }

    return otRet;
}

/*
 return 0 if value contains only 1 bit set
 1, 2 4 8 16 32 64 128 512 ....
 */
bool is_power_of_two(uint32_t const val) {
    bool outRetPt = false;
    /*check power of two*/
    if(0u == (val & (val - 1u))) {
        /*only one bit set in the value */
        outRetPt = true;
    }
    if(0u == val) {
        outRetPt = false;
    }
    return outRetPt;
}

uint8_t get_bit_from_32bit(uint32_t in_val, uint8_t bit_num) {
    uint8_t bit = 0xFF;
    if(bit_num < 32) {
        bit = MASK_1BIT & (in_val >> bit_num);
    }
    return bit;
}

uint16_t invert_bits16(uint16_t val) { return MASK_16BIT ^ val; }
