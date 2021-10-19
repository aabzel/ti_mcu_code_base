#ifndef BOOT_COMMANDS_H
#define BOOT_COMMANDS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "ostream.h"

bool bool_launch_app_command(int32_t argc, char* argv[]);
bool boot_jump_addr_command(int32_t argc, char* argv[]);
bool boot_diag_command(int32_t argc, char* argv[]);
bool bool_erase_app_command(int32_t argc, char* argv[]);

#define BOOT_COMMANDS                                                                                                  \
    SHELL_CMD("boot_diag", "bd", boot_diag_command, "Boot diag"),                                                      \
        SHELL_CMD("jump", "jm", boot_jump_addr_command, "jump to address"),                                            \
        SHELL_CMD("launch", "la", bool_launch_app_command, "launch app"),                                              \
        SHELL_CMD("erase_app", "ea", bool_erase_app_command, "Erase application"),

#ifdef __cplusplus
}
#endif

#endif /* BOOT_COMMANDS_H */
