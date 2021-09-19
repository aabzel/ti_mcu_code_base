#ifndef CLOCK_COMMANDS_H
#define CLOCK_COMMANDS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "ostream.h"

bool sw_pause_command(int32_t argc, char* argv[]);
bool systick_set_command(int32_t argc, char* argv[]);
bool clock_diag_command(int32_t argc, char* argv[]);

#define CLOCK_COMMANDS                                                                                                 \
    SHELL_CMD("clock_diag", "cld", clock_diag_command, "Clock diag"),                                                  \
        SHELL_CMD("sw_pause", "swp", sw_pause_command, "SW pause"),                                                    \
        SHELL_CMD("systic_set", "sts", systick_set_command, "SysTic set period"),

#ifdef __cplusplus
}
#endif

#endif /* CLOCK_COMMANDS_H */
