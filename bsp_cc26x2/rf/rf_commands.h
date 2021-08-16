#ifndef RF_COMMANDS_H
#define RF_COMMANDS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "cli_manager.h"

bool rf_init_command(int32_t argc, char* argv[]);
bool rf_write_command(int32_t argc, char* argv[]);
bool rf_read_command(int32_t argc, char* argv[]);
bool rf_diag_command(int32_t argc, char* argv[]);

#define RF_COMMANDS                                                                                                    \
    SHELL_CMD("rf_diag", "rfd", rf_diag_command, "RF diag"),                                                           \
        SHELL_CMD("rf_write", "rfw", rf_write_command, "RF write hex str"),                                            \
        SHELL_CMD("rf_read", "rfr", rf_read_command, "RF read hex str"),                                               \
        SHELL_CMD("rf_init", "rfi", rf_init_command, "RF init"),

#ifdef __cplusplus
}
#endif

#endif /* RF_COMMANDS_H */
