#ifndef BOOT_COMMANDS_H
#define BOOT_COMMANDS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "ostream.h"

#ifdef HAS_BOOTLOADER
bool boot_diag_command(int32_t argc, char* argv[]);
bool bool_erase_app_command(int32_t argc, char* argv[]);
bool boot_jump_addr_command(int32_t argc, char* argv[]);
bool bool_launch_app_command(int32_t argc, char* argv[]);

#define BOOT_COMMANDS_FOR_BOOT                                                                                         \
        SHELL_CMD("jump", "jm", boot_jump_addr_command, "jump to address"),                                            \
        SHELL_CMD("launch", "la", bool_launch_app_command, "launch app"),                                              \
        SHELL_CMD("erase_app", "ea", bool_erase_app_command, "Erase application"),                                     \
        SHELL_CMD("boot_diag", "bd", boot_diag_command, "Boot diag"),
#else
#define BOOT_COMMANDS_FOR_BOOT
#endif

#ifdef HAS_GENERIC
bool boot_jump_boot_command(int32_t argc, char* argv[]);

#define BOOT_COMMANDS_FOR_APP SHELL_CMD("jump_boot", "jb", boot_jump_boot_command, "jump boot"),
#else
#define BOOT_COMMANDS_FOR_APP
#endif

#define BOOT_COMMANDS                                                                                                  \
    BOOT_COMMANDS_FOR_BOOT                                                                                             \
    BOOT_COMMANDS_FOR_APP

#ifdef __cplusplus
}
#endif

#endif /* BOOT_COMMANDS_H */
