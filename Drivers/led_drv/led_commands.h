#ifndef LED_COMMANDS_H
#define LED_COMMANDS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "ostream.h"

bool led_get_command(int32_t argc, char* argv[]);
bool led_set_command(int32_t argc, char* argv[]);

#define LED_COMMANDS                                                                                                   \
    SHELL_CMD("led_get", "lg", led_get_command, "LED get"), SHELL_CMD("led_set", "ls", led_set_command, "LED set"),

#ifdef __cplusplus
}
#endif

#endif /* LED_COMMANDS_H */
