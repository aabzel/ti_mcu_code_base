#ifndef BOOT_COMMANDS_H
#define BOOT_COMMANDS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "ostream.h"

bool boot_diag_command(int32_t argc, char* argv[]);

#define BOOT_COMMANDS SHELL_CMD("boot_diag", "btd", boot_diag_command, "Boot diag"),

#ifdef __cplusplus
}
#endif

#endif /* BOOT_COMMANDS_H */
