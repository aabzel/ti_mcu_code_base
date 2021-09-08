#ifndef TIM_COMMANDS_H
#define TIM_COMMANDS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "ostream.h"

bool tim_diag_command(int32_t argc, char* argv[]);
bool tim_set_prescaler_command(int32_t argc, char* argv[]);
bool tim_set_laod_command(int32_t argc, char* argv[]);
bool tim_set_period_command(int32_t argc, char* argv[]);

#define TIM_COMMANDS \
    SHELL_CMD("tim_diag", "td", tim_diag_command, "TIM diag"), \
    SHELL_CMD("tim_set_psc", "tsp", tim_set_prescaler_command, "TIM prescaler"), \
    SHELL_CMD("tim_set_load", "tsl", tim_set_laod_command, "TIM load"), \
    SHELL_CMD("tim_set_per", "tspr", tim_set_period_command, "TIM period"),

#ifdef __cplusplus
}
#endif

#endif /* TIM_COMMANDS_H */
