#ifndef GPIO_COMMANDS_H
#define GPIO_COMMANDS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "ostream.h"

bool gpio_list_command(int32_t argc, char* argv[]);
bool gpio_get_command(int32_t argc, char* argv[]);
bool gpio_set_command(int32_t argc, char* argv[]);
bool gpio_toggle_command(int32_t argc, char* argv[]);

#define GPIO_COMMANDS                                                                                                  \
    SHELL_CMD("gpio_list", "gl", gpio_list_command, "GPIO list"),                                                      \
    SHELL_CMD("gpio_toggle", "gt", gpio_toggle_command, "GPIO toggle"),                                                      \
        SHELL_CMD("gpio_get", "gg", gpio_get_command, "GPIO get"),                                                     \
        SHELL_CMD("gpio_set", "gs", gpio_set_command, "GPIO set"),

#ifdef __cplusplus
}
#endif

#endif /* GPIO_COMMANDS_H */
