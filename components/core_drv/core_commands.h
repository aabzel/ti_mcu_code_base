#ifndef CORE_COMMANDS_H
#define CORE_COMMANDS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "ostream.h"

bool core_diag_command(int32_t argc, char* argv[]);

#define CORTEX_M4_COMMANDS                                                                                                  \
    SHELL_CMD("core_diag", "cd", core_diag_command, "Cortex M4 diag"),                                                      

#ifdef __cplusplus
}
#endif

#endif /* CORE_COMMANDS_H */
