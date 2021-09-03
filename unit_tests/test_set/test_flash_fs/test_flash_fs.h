#ifndef FLASH_FS_TEST_H
#define FLASH_FS_TEST_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

bool test_flash_fs_1(void);
#ifdef HAS_RAND
bool test_flash_fs_2(void);
bool test_flash_fs_3(void);
#endif

#define TEST_SUIT_FLASH_FS {"flash_fs_1", test_flash_fs_1},

#ifdef __cplusplus
}
#endif

#endif /* FLASH_FS_TEST_H */
