#ifndef FLASH_NVS_COMMANDS_H
#define FLASH_NVS_COMMANDS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

bool flash_nvs_diag_command(int32_t argc, char* argv[]);
bool flash_nvs_erase_command(int32_t argc, char* argv[]);
bool flash_nvs_read_command(int32_t argc, char* argv[]);
bool flash_nvs_write_command(int32_t argc, char* argv[]);

#define FLASH_NVS_COMMANDS                                                                                         \
    SHELL_CMD("flash_nvs_diag", "fnd", flash_nvs_diag_command, "Flash diag"),                                      \
    SHELL_CMD("flash_nvs_erase", "fne", flash_nvs_erase_command, "Flash erase NVS"),                               \
    SHELL_CMD("flash_nvs_read", "fnr", flash_nvs_read_command, "Flash read"),                                      \
    SHELL_CMD("flash_nvs_write", "fnw", flash_nvs_write_command, "Flash write NVS"),

#ifdef __cplusplus
}
#endif

#endif /* FLASH_NVS_COMMANDS_H */
