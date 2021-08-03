#ifndef I2C_COMMANDS_H
#define I2C_COMMANDS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "ostream.h"

bool i2c_send_command(int32_t argc, char* argv[]);
bool i2c_diag_command(int32_t argc, char* argv[]);

#define I2C_COMMANDS                                                                  \
        SHELL_CMD("i2c_diag", "i2d", i2c_diag_command, "I2C diag"),                    \
        SHELL_CMD("i2c_send", "i2s", i2c_send_command, "I2C send hex str"),

#ifdef __cplusplus
}
#endif

#endif /* I2C_COMMANDS_H */
