#ifndef FLASH_FS_COMMANDS_H
#define FLASH_FS_COMMANDS_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

bool cmd_flash_fs_toggle_page(int32_t argc, char* argv[]);
bool cmd_flash_fs_inval(int32_t argc, char* argv[]);
bool cmd_flash_fs_diag(int32_t argc, char* argv[]);
bool cmd_flash_fs_get(int32_t argc, char* argv[]);
bool cmd_flash_fs_get_addr(int32_t argc, char* argv[]);
bool cmd_flash_fs_set(int32_t argc, char* argv[]);
bool cmd_flash_fs_scan(int32_t argc, char* argv[]);
bool cmd_flash_fs_format(int32_t argc, char* argv[]);

#define FLASH_FS_COMMANDS                                                                                              \
    SHELL_CMD("flash_fs_diag", "ffd", cmd_flash_fs_diag, "Flash FS diag"),                                             \
        SHELL_CMD("flash_fs_inval", "ffi", cmd_flash_fs_inval, "Flash FS invalidate"),                                 \
        SHELL_CMD("flash_fs_toggle_page", "fft", cmd_flash_fs_toggle_page, "Flash FS toggle page"),                    \
        SHELL_CMD("flash_fs_scan", "ffc", cmd_flash_fs_scan, "Flash FS scan"),                                         \
        SHELL_CMD("flash_fs_get", "ffg", cmd_flash_fs_get, "Flash FS get"),                                            \
        SHELL_CMD("flash_fs_get_add", "fga", cmd_flash_fs_get_addr, "Flash FS get addrass"),                           \
        SHELL_CMD("flash_fs_set", "ffs", cmd_flash_fs_set, "Flash FS set"),                                            \
        SHELL_CMD("flash_fs_format", "fff", cmd_flash_fs_format, "Flash FS format"),

#ifdef __cplusplus
}
#endif

#endif /* FLASH_FS_COMMANDS_H */
