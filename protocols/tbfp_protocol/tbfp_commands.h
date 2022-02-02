#ifndef TBFP_COMMAND_H
#define TBFP_COMMAND_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "cli_manager.h"
bool tbfp_error_command(int32_t argc, char* argv[]);
bool tbfp_diag_command(int32_t argc, char* argv[]);
bool tbfp_send_command(int32_t argc, char* argv[]);
bool tbfp_send_hi_load_command(int32_t argc, char* argv[]);

#define TBFP_COMMANDS                                                                                                  \
    SHELL_CMD("tbfp_diag", "tbd", tbfp_diag_command, "TBFP diag"),                                                     \
    SHELL_CMD("tbfp_error", "tbe", tbfp_error_command, "TBFP error"),                                                  \
        SHELL_CMD("tbfp_send_h", "tbsh", tbfp_send_hi_load_command, "TBFP SendHiLoad"),                                \
        SHELL_CMD("tbfp_send", "tbs", tbfp_send_command, "TBFP send"),

#ifdef __cplusplus
}
#endif

#endif /* TBFP_COMMAND_H */
