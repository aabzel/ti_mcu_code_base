#ifndef GPIO_PWM_COMMANDS_H
#define GPIO_PWM_COMMANDS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "cli_manager.h"

bool gpio_pwm_diag_command(int32_t argc, char* argv[]);
bool gpio_pwm_set_command(int32_t argc, char* argv[]);

#define GPIO_PWM_COMMANDS                                                                                              \
    SHELL_CMD("gpio_pwm_cmd", "gpwm", gpio_pwm_set_command, "GPIO PWM set"),                                           \
        SHELL_CMD("gpio_pwm_diag", "gpwmd", gpio_pwm_diag_command, "GPIO PWM diag"),

#ifdef __cplusplus
}
#endif

#endif /* GPIO_PWM_COMMANDS_H */
