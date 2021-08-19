#ifndef CRC_16_CCITT_FALSE_H
#define CRC_16_CCITT_FALSE_H

#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

uint16_t calc_crc16_ccitt_false(const uint8_t* in_data, uint16_t const length);

#ifdef __cplusplus
}
#endif

#endif /* CRC_16_CCITT_FALSE_H */
