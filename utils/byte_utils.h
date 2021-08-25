#ifndef BYTE_UTILS_H
#define BYTE_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

uint64_t reverse_byte_order_uint64(const uint64_t in4byteVal);
uint32_t reverse_byte_order_uint32(const uint32_t in4byteVal);
uint32_t reverse_byte_order_uint24(const uint32_t in3byteVal);
uint16_t reverse_byte_order_uint16(const uint16_t in2byteVal);

#ifdef __cplusplus
}
#endif

#endif /* BYTE_UTILS_H */
