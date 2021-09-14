#ifndef CRC_16_CCITT_FALSE_H
#define CRC_16_CCITT_FALSE_H

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

uint16_t calc_crc16_ccitt_false(const uint8_t* in_data, uint32_t const length);
bool crc16_check(const uint8_t* in_data, uint32_t const length, uint32_t crc16_read);

#ifdef __cplusplus
}
#endif

#endif /* CRC_16_CCITT_FALSE_H */
