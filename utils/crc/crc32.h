#ifndef CRC32_H
#define CRC32_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

uint32_t crc32(const uint8_t data_8[], uint32_t len_8);

#ifdef __cplusplus
}
#endif

#endif /* CRC32_H */
