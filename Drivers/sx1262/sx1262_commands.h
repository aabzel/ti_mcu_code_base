#ifndef SX1262_COMMANDS_H
#define SX1262_COMMANDS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "cli_manager.h"

bool sx1262_init_command(int32_t argc, char* argv[]);
bool sx1262_diag_command(int32_t argc, char* argv[]);

#define SX1262_COMMANDS                                                                                                \
    SHELL_CMD("lora_diag", "sxd", sx1262_diag_command, "SX1262 diag"),                                                 \
        SHELL_CMD("lora_read", "sxi", sx1262_init_command, "SX1262 init"),

#ifdef __cplusplus
}
#endif

#endif /* SX1262_COMMANDS_H */
