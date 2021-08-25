#ifndef SRC_DATA_UTILS_H
#define SRC_DATA_UTILS_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

/* Compile time assert */
/* Note we need the 2 concats below because arguments to ##
 * are not expanded, so we need to expand __LINE__ with one indirection
 * before doing the actual concatenation. */
#define ASSERT_CONCAT_(a, b) a##b
#define ASSERT_CONCAT(a, b) ASSERT_CONCAT_(a, b)
#define ct_assert(e)                                                           \
  {                                                                            \
    enum { ASSERT_CONCAT(assert_line_, __LINE__) = 1 / (!!(e)) };              \
  }

typedef union uAllTypeUnion_t{
    uint64_t u64;
    int64_t  s64;
    uint32_t u32[2];
    int32_t s32[2];
    float f32[2];
    uint16_t u16[4];
    int16_t s16[4];
    uint8_t u8[8];
    int8_t s8[8];
} AllTypeUnion_t;

typedef union uType32Union_t{
    uint32_t u32;
    int32_t s32;
    float f32;
    uint16_t u16[2];
    int16_t s16[2];
    uint8_t u8[4];
    int8_t s8[4];
} Type32Union_t;

typedef union uType16Union_t{
    uint16_t u16;
    int16_t s16;
    uint8_t u8[2];
    int8_t s8[2];
} Type16Union_t;
  
uint8_t rx_max8u(uint8_t max8u_x1, uint8_t max8u_x2);
uint8_t rx_min8u(uint8_t min8u_x1, uint8_t min8u_x2);
uint16_t rx_max16u(uint16_t max16u_x1, uint16_t max16u_x2);
uint16_t rx_min16u(uint16_t min16u_x1, uint16_t min16u_x2);
uint32_t rx_max32u(uint32_t max32u_x1, uint32_t max32u_x2);
uint32_t rx_min32u(uint32_t min32u_x1, uint32_t min32u_x2);
int8_t rx_max8(int8_t max8_x1, int8_t max8_x2);
int8_t rx_min8(int8_t min8_x1, int8_t min8_x2);
int16_t rx_max16(int16_t max16_x1, int16_t max16_x2);
int16_t rx_min16(int16_t min16_x1, int16_t min16_x2);
int32_t rx_max32(int32_t max32_x1, int32_t max32_x2);
int32_t rx_min32(int32_t min32_x1, int32_t min32_x2);
uint32_t swap32(uint32_t u32_num);
uint16_t swap16(uint16_t u16_num);
const char *get_spaces_str(uint32_t spaces_length);
float ms2Hz(uint16_t ms);
uint8_t uint16_to_uint8_limiter(uint16_t in_val);
int8_t two_complement_to_decimal(uint8_t in_code, int significant_bits);
uint8_t extract_digit(uint32_t in_num ,uint8_t digit_index);
bool is_arr_pat (uint8_t *arr, uint32_t size, uint8_t patt);
bool try_alloc_on_stack(int n, uint8_t pat) ;

#ifdef __cplusplus
}
#endif

#endif /* SRC_DATA_UTILS_H */
