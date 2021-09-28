#ifndef BQ25171_Q1_COMMANDS_H
#define BQ25171_Q1_COMMANDS_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

bool bq25171_q1_diag_command(int32_t argc, char* argv[]);
bool bq25171_q1_ctrl_command(int32_t argc, char* argv[]);

#define BQ25171_Q1_COMMANDS                                                                                            \
    SHELL_CMD("bat_chare_diag", "bcd", bq25171_q1_diag_command, "Battery Charger diag"),                               \
        SHELL_CMD("bat_chare_ctrl", "bcc", bq25171_q1_ctrl_command, "Battery Charger  enable"),

#ifdef __cplusplus
}
#endif

#endif /* BQ25171_Q1_COMMANDS_H */
