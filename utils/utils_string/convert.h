#ifndef STR_CONVERT_H
#define STR_CONVERT_H

#ifdef __cplusplus
 "C" {
#endif

#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "float_utils.h"


#define MAX_INT64_STR_LEN_10 (sizeof("-18446744073709551614") - 1U)
#define MAX_INT32_STR_LEN_10 (sizeof("-4294967295") - 1U)

#define MAX_INT64_STR_LEN_16 (sizeof(uint64_t) * 2U)
#define MAX_INT32_STR_LEN_16 (sizeof(uint32_t) * 2U)
#define MAX_INT16_STR_LEN_16 (sizeof(uint16_t) * 2U)
#define MAX_INT8_STR_LEN_16 (sizeof(uint8_t) * 2U)

#define MAX_PRECISION (10U)

bool is_dec_str(const char str_to_check[], int32_t str_to_check_len);
bool is_hex_str(const char str_to_check[], int32_t str_to_check_len, uint8_t* const out_shift);
bool try_str2uint64(const char u64_str[], uint64_t* u64_value);
bool try_str2int64(const char s64_str[], int64_t* s64_value);
bool try_strl2uint64(const char u64l_str[], int32_t u64l_str_len, uint64_t* u64l_value);
bool try_strl2int64(const char s64l_str[], int32_t s64l_str_len, int64_t* s64l_value);
bool try_strl2uint64_hex(const char u64_hex_str[], int32_t u64_hex_str_len, uint64_t* u64_hex_value);
bool try_strl2int64_hex(const char s64_hex_str[], int32_t s64_hex_str_len, int64_t* s64_hex_value);
bool try_strl2uint64_dec(const char u64_dec_str[], int32_t u64_dec_str_len, uint64_t* u64_dec_value);
bool try_strl2int64_dec(const char s64_dec_str[], int32_t s64_dec_str_len, int64_t* s64_dec_value);
bool try_str2uint32(const char u32_str[], uint32_t* u32_value);
bool try_str2int32(const char s32_str[], int32_t* s32_value);
bool try_strl2uint32(const char u32l_str[], int32_t u32l_str_len, uint32_t* u32l_value);
bool try_strl2int32(const char s32l_str[], int32_t s32l_str_len, int32_t* s32l_value);
bool try_str2uint32_hex(const char u32_hex_str[], uint32_t* u32_hex_value);
bool try_strl2uint32_hex(const char u32l_hex_str[], int32_t u32l_hex_str_len, uint32_t* u32l_hex_value);
bool try_strl2int32_hex(const char s32l_hex_str[], int32_t s32l_hex_str_len, int32_t* s32l_hex_value);
bool try_strl2uint32_dec(const char u32_dec_str[], int32_t u32_dec_str_len, uint32_t* u32_dec_value);
bool try_strl2int32_dec(const char s32_dec_str[], int32_t s32_dec_str_len, int32_t* s32_dec_value);
bool try_str2uint16(const char u16_str[], uint16_t* u16_value);
bool try_str2int16(const char s16_str[], int16_t* s16_value);
bool try_strl2uint16(const char u16l_str[], int32_t u16l_str_len, uint16_t* u16l_value);
bool try_strl2int16(const char s16l_str[], int32_t s16l_str_len, int16_t* s16l_value);
bool try_strl2uint16_hex(const char u16l_hex_str[], int32_t u16l_hex_str_len, uint16_t* u16l_hex_value);
bool try_str2uint8(const char u8_str[], uint8_t* u8_value);
bool try_str2int8(const char s8_str[], int8_t* s8_value);
bool try_strl2uint8(const char u8l_str[], int32_t u8l_str_len, uint8_t* u8l_value);
bool try_strl2int8(const char s8l_str[], int32_t s8l_str_len, int8_t* s8l_value);
bool try_strl2uint8_hex(const char u8l_hex_str[], int32_t u8l_hex_str_len, uint8_t* u8l_hex_value);
bool try_strl2int8_hex(const char s8l_hex_str[], int32_t s8l_hex_str_len, int8_t* s8l_hex_value);
bool try_str2bool(const char bool_str[], bool* bool_value);
bool try_str2float(const char float_str[], float* float_value);
bool try_str2double(const char double_str[], double* double_value);
bool try_strl2double(const char double_str[], int32_t u8l_hex_str_len, double* double_value);
bool try_strl2array(char* in_str_array, uint32_t len_str_in, uint8_t* out_array, uint16_t array_size, uint32_t* out_array_len);
bool try_str2array(char* in_str_array, uint8_t* out_array, uint16_t array_size, uint32_t* out_array_len);
const char* ltoa32_(int32_t s32_data, char s32_stringified[], uint8_t s32_base, uint32_t* s32_len);
const char* ltoa64_(int64_t s64_data, char s64_stringified[], uint8_t s64_base, uint32_t* s64_len);
const char* utoa_bin8(uint8_t u8_bin_data);
const char* utoa32_(uint32_t u32_data, char u32_stringified[], uint8_t u32_base, uint32_t* u32_len);
const char* utoa64_(uint64_t u64_data, char u64_stringified[], uint8_t u64_base, uint32_t* u64_len);
void dtoa_(double double_data_, int32_t double_precision_, char double_stringified_[]);
void ftoa_(float_t float_data_, int32_t float_precision_, char float_stringified_[]);
uint32_t base64_encode(const uint8_t encode_data[], uint32_t encode_data_size, char encode_result[]);
bool base64_decode(const char decode_str[], int32_t decode_str_len, uint8_t decode_data[], uint32_t* decode_data_size);
const char* rx_ltoa32(int32_t ltoa32_data);
const char* rx_ltoa64(int64_t ltoa64_data);
const char* rx_utoa32(uint32_t utoa32_data);
const char* rx_utoa64(uint64_t utoa64_data);
const char* utoa_hex32(uint32_t u32_hex_data);
const char* utoa_hex64(uint64_t u64_hex_data);
const char* ltoa_hex32(int32_t s32_hex_data);
const char* ltoa_hex64(int64_t s64_hex_data);
const char* utoa_bin16(uint16_t u16_bin_data);
const char* utoa_bin24(uint32_t u32_bin_data);
const char* utoa_bin32(uint32_t u32_bin_data);
uint16_t float_to_uint16(float val);
uint32_t assemble_uint32(uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4);
const char* rx_dtoa(double d);
const char* rx_ftoa(float_t float_v);
const char* bool2name(bool val);
const char* bool2test_status(bool val);

#ifdef __cplusplus
}
#endif

#endif /* STR_CONVERT_H */
