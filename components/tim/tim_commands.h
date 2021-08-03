#ifndef TIM_COMMANDS_H
#define TIM_COMMANDS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "ostream.h"

bool tim_diag_command(int32_t argc, char* argv[]);

#define TIM_COMMANDS                                                                  \
        SHELL_CMD("tim_diag", "td", tim_diag_command, "TIM diag"),                    

#ifdef __cplusplus
}
#endif

#endif /* TIM_COMMANDS_H */
