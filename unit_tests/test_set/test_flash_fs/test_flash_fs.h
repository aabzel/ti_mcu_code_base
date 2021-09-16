#ifndef FLASH_FS_TEST_H
#define FLASH_FS_TEST_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

bool test_flash_fs_set_get_const(void);
bool test_flash_fs_set_get_set(void);
bool test_flash_fs_inval(void);
bool test_flash_fs_toggle(void);

#define TEST_SUIT_FLASH_FS                                                                                             \
    {"flash_fs_set_get_const", test_flash_fs_set_get_const}, {"test_flash_fs_set_get_set", test_flash_fs_set_get_set}, \
        {"flash_fs_inval", test_flash_fs_inval}, {"flash_fs_toggle", test_flash_fs_toggle},

#ifdef __cplusplus
}
#endif

#endif /* FLASH_FS_TEST_H */
