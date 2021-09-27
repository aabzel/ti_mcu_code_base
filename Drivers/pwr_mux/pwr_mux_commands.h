#ifndef PWR_MUX_COMMANDS_H
#define PWR_MUX_COMMANDS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "cli_manager.h"

bool pwr_mux_set_command(int32_t argc, char* argv[]);
bool pwr_mux_diag_command(int32_t argc, char* argv[]);

#define PWR_MUX_COMMANDS                                                                                                   \
    SHELL_CMD("power_mux_diag", "pmd", pwr_mux_diag_command, "power mux diag"),                                                  \
    SHELL_CMD("power_mux_set", "pms", pwr_mux_set_command, "power mux set"),

#ifdef __cplusplus
}
#endif

#endif /* PWR_MUX_COMMANDS_H */
