#ifndef BMP180_COMMANDS_H
#define BMP180_COMMANDS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "cli_manager.h"

bool bmp180_read_reg_command(int32_t argc, char* argv[]);
bool bmp180_diag_command(int32_t argc, char* argv[]);

#define BMP180_COMMANDS                                                                                                   \
    SHELL_CMD("bmp180_diag", "bmd", bmp180_diag_command, "BMP180 diag"),                                                  \
    SHELL_CMD("bmp180_read", "bmr", bmp180_read_reg_command, "BMP180 read reg"),

#ifdef __cplusplus
}
#endif

#endif /* BMP180_COMMANDS_H */
