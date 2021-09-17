#ifndef FLASH_TEST_H
#define FLASH_TEST_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#define FLASH_ADDR_TEST 0x00048000

bool test_flash_write(void);

#define TEST_SUIT_FLASH   {"flash_write", test_flash_write},

#ifdef __cplusplus
}
#endif

#endif /* FLASH_TEST_H */
