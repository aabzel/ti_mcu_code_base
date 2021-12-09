#ifndef INTERRUPT_COMMANDS_H
#define INTERRUPT_COMMANDS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "ostream.h"

bool it_diag_command(int32_t argc, char* argv[]);

#define INTERRUPT_COMMANDS SHELL_CMD("it_diag", "itd", it_diag_command, "IT diag"),

#ifdef __cplusplus
}
#endif

#endif /* INTERRUPT_COMMANDS_H */
