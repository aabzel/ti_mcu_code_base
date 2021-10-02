#ifndef TEST_CRC_H
#define TEST_CRC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

typedef struct xCrcTestCase_t {
    uint8_t len;
    uint8_t buf[9];
    uint8_t crc8;
} Crc8TestCase_t;

bool test_crc8(void);
bool test_crc32(void);
bool test_crc24q13(void);
bool test_crc24q80(void);
bool test_crc16(void);

#define CRC_TEST_SUIT \
    {"crc32", test_crc32}, \
    {"crc8", test_crc8}, \
    {"crc16", test_crc16}, \
    {"crc24_13", test_crc24q13},\
    {"crc24_80", test_crc24q80},

#ifdef __cplusplus
}
#endif

#endif /* TEST_CRC_H */
