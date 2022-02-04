#ifndef PARAM_COMMANDS_H
#define PARAM_COMMANDS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

bool cmd_param_get(int32_t argc, char* argv[]);
bool cmd_param_set(int32_t argc, char* argv[]);
bool cmd_param_diag(int32_t argc, char* argv[]);

#define PARAM_COMMANDS                                                                                                 \
    SHELL_CMD("param_get", "pg", cmd_param_diag, "Param diag"),                                                        \
    SHELL_CMD("param_get_id", "pgi", cmd_param_get, "Param get"),                                                        \
        SHELL_CMD("param_set", "ps", cmd_param_set, "Param set"),

#ifdef __cplusplus
}
#endif

#endif /* PARAM_COMMANDS_H  */
