#ifndef RTCM3_COMMAND_H
#define RTCM3_COMMAND_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "cli_manager.h"

bool rtcm3_diag_command(int32_t argc, char* argv[]);
bool rtcm3_reset_command(int32_t argc, char* argv[]);

#define RTCM3_COMMANDS                                                                                              \
    SHELL_CMD("rtcm_diag", "rtcmd", rtcm3_diag_command, "RTCM3 diag"),                                              \
    SHELL_CMD("rtcm_reset", "rtcmr", rtcm3_reset_command, "RTCM3 reset"),

#ifdef __cplusplus
}
#endif

#endif /* RTCM3_COMMAND_H */
