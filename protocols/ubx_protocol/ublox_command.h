#ifndef U_BLOX_COMMAND_H
#define U_BLOX_COMMAND_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "cli_manager.h"

bool ubx_nav_command(int32_t argc, char* argv[]);
bool ubx_print_key_val_command(int32_t argc, char* argv[]);
bool ubx_diag_command(int32_t argc, char* argv[]);
bool ubx_send_command(int32_t argc, char* argv[]);
bool ubx_get_key_command(int32_t argc, char* argv[]);

#define UBLOX_COMMANDS                                                                                                 \
    SHELL_CMD("ubx_diag", "ubd", ubx_diag_command, "U-Blox diag"),                                                     \
    SHELL_CMD("ubx_nav", "ubn", ubx_nav_command, "U-Blox navigation result"),                                     \
        SHELL_CMD("ubx_send", "ubs", ubx_send_command, "U-Blox Send packet"),                                          \
        SHELL_CMD("ubx_key_val", "ubk", ubx_print_key_val_command, "U-Blox key val"),                                  \
        SHELL_CMD("ubx_getey", "ubg", ubx_get_key_command, "U-Blox get key"),

#ifdef __cplusplus
}
#endif

#endif /* U_BLOX_COMMAND_H */
