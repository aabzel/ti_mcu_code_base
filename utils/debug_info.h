#ifndef DEBUG_INFO_H
#define DEBUG_INFO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "ostream.h"

typedef union xU8_bit_t {
    uint8_t u8;
    int8_t s8;
} U8_bit_t;

typedef union xU16_bit_t {
    int16_t s16;
    uint16_t u16;
    uint8_t u8[2];
    int8_t s8[2];
} U16_bit_t;

typedef union xU32_bit_t {
    uint32_t u32;
    int32_t s32;
    float f32;
    uint16_t u16[2];
    int16_t s16[2];
    uint8_t u8[4];
    int8_t s8[4];
} U32_bit_t;

typedef union xU64_bit_t {
    uint64_t u64;
    int64_t s64;
    uint32_t u32[2];
    int32_t s32[2];
    float d64;
    float f32[2];
    uint16_t u16[4];
    int16_t s16[4];
    uint8_t u8[8];
    int8_t s8[8];
} U64_bit_t;

bool explore_stack_dir(void);
bool is_little_endian(void);
bool print_16bit_types(void* val);
bool print_version(void);
void print_version_s(ostream_t* stream);
bool print_vector_table(uint32_t vectors_table_base);
void print_sys_info(void);
bool find_addr_by_val(uint16_t byte_num, uint32_t val, uint32_t start_addr, uint32_t end_addr);
bool print_mem(uint8_t* addr, int32_t len, bool new_line);
bool print_bit_hint(uint16_t offset, uint32_t bitness);
bool print_bit_representation(uint32_t val);

#ifdef __cplusplus
}
#endif

#endif /* DEBUG_INFO_H */
