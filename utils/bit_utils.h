#ifndef BIT_UTILS_H
#define BIT_UTILS_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define BIT_31 (1U << 31)
#define BIT_30 (1U << 30)
#define BIT_29 (1U << 29)
#define BIT_28 (1U << 28)
#define BIT_27 (1U << 27)
#define BIT_26 (1U << 26)
#define BIT_25 (1U << 25)
#define BIT_24 (1U << 24)
#define BIT_23 (1U << 23)
#define BIT_22 (1U << 22)
#define BIT_21 (1U << 21)
#define BIT_20 (1U << 20)
#define BIT_19 (1U << 19)
#define BIT_18 (1U << 18)
#define BIT_17 (1U << 17)
#define BIT_16 (1U << 16)
#define BIT_15 (1U << 15)
#define BIT_14 (1U << 14)
#define BIT_13 (1U << 13)
#define BIT_12 (1U << 12)
#define BIT_11 (1U << 11)
#define BIT_10 (1U << 10)
#define BIT_9 (1U << 9)
#define BIT_8 (1U << 8)
#define BIT_7 (1U << 7)
#define BIT_6 (1U << 6)
#define BIT_5 (1U << 5)
#define BIT_4 (1U << 4)
#define BIT_3 (1U << 3)
#define BIT_2 (1U << 2)
#define BIT_1 (1U << 1)
#define BIT_0 (1U << 0)

#define MASK_0BIT (uint32_t)0x00000000U
#define MASK_1BIT (uint32_t)0x00000001U
#define MASK_2BIT (uint32_t)0x00000003U
#define MASK_3BIT (uint32_t)0x00000007U
#define MASK_4BIT (uint32_t)0x0000000FU
#define MASK_5BIT (uint32_t)0x0000001FU
#define MASK_6BIT (uint32_t)0x0000003FU
#define MASK_7BIT (uint32_t)0x0000007FU
#define MASK_8BIT (uint32_t)0x000000FFU
#define MASK_9BIT (uint32_t)0x000001FFU
#define MASK_10BIT (uint32_t)0x000003FFU
#define MASK_11BIT (uint32_t)0x000007FFU
#define MASK_12BIT (uint32_t)0x00000FFFU
#define MASK_13BIT (uint32_t)0x00001FFFU
#define MASK_14BIT (uint32_t)0x00003FFFU
#define MASK_15BIT (uint32_t)0x00007FFFU
#define MASK_16BIT (uint32_t)0x0000FFFFU
#define MASK_17BIT (uint32_t)0x0001FFFFU
#define MASK_18BIT (uint32_t)0x0003FFFFU
#define MASK_19BIT (uint32_t)0x0007FFFFU
#define MASK_20BIT (uint32_t)0x000FFFFFU
#define MASK_21BIT (uint32_t)0x001FFFFFU
#define MASK_22BIT (uint32_t)0x003FFFFFU
#define MASK_23BIT (uint32_t)0x007FFFFFU
#define MASK_24BIT (uint32_t)0x00FFFFFFU
#define MASK_25BIT (uint32_t)0x01FFFFFFU
#define MASK_30BIT (uint32_t)0x3FFFFFFFU

#define MASK_50BIT ((uint64_t)0x0003FFFFFFFFFFFF)

#define SET_BIT_NUM(VAL, BIT) ((VAL) |= (1U << (BIT)))
#define RESET_BIT_NUM(VAL, BIT) ((VAL) &= ~(1U << (BIT)))
#define CHECK_BIT_NUM(VAL, BIT) ((1U << (BIT)) == ((VAL) & (1U << (BIT))))

uint32_t generate_32bit_custom_mask(uint8_t max_bit, uint8_t min_bit);

uint32_t generate_32bit_left_mask(uint8_t bitlen);
uint32_t extract_subval_from_32bit(uint32_t inVal, uint8_t maxBit, uint8_t minBit);

uint64_t generate_64bit_left_mask(uint8_t bitlen);
uint64_t extract_subval_from_64bit(uint64_t inVal, uint8_t maxBit, uint8_t minBit);

uint32_t insert_subval_in_32bit(uint32_t orig_val, uint32_t sub_val, uint8_t max_bit, uint8_t min_bit);

bool bit32_control_proc(uint32_t *address_val, char cmd, uint8_t bit);

#ifdef __cplusplus
}
#endif

#endif /* BIT_UTILS_H */
