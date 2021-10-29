#ifndef ZED_F9P_COMMANDS_H
#define ZED_F9P_COMMANDS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "ostream.h"

bool zed_f9p_diag_command(int32_t argc, char* argv[]);

#define ZED_F9P_COMMANDS                                                                                                   \
    SHELL_CMD("zed_f9p_diag", "zfd", zed_f9p_diag_command, "ZED F9P diag"),


#ifdef __cplusplus
}
#endif

#endif /* ZED_F9P_COMMANDS_H */
