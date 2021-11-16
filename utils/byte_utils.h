#ifndef BYTE_UTILS_H
#define BYTE_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

uint64_t reverse_byte_order_uint64(const uint64_t in4byteVal);
uint32_t reverse_byte_order_uint32(const uint32_t in4byteVal);
uint32_t reverse_byte_order_uint24(const uint32_t in3byteVal);
uint16_t reverse_byte_order_uint16(const uint16_t in2byteVal);
bool reverse_byte_order_array(uint8_t * in_out_array, uint32_t len);

uint16_t copy_and_rev16(const uint8_t *const array);
uint32_t copy_and_rev32(const uint8_t *const array);
uint64_t copy_and_rev64(const uint8_t *const array);

#ifdef __cplusplus
}
#endif

#endif /* BYTE_UTILS_H */
