#ifndef RTC_COMMANDS_H
#define RTC_COMMANDS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "ostream.h"

bool rtc_set_sec_command(int32_t argc, char* argv[]);
bool rtc_reset_command(int32_t argc, char* argv[]);
bool rtc_disable_command(int32_t argc, char* argv[]);
bool rtc_diag_command(int32_t argc, char* argv[]);

#define RTC_COMMANDS                                                 \
        SHELL_CMD("rtc_diag", "rd", rtc_diag_command, "RTC diag"),   \
        SHELL_CMD("rtc_disable", "rdi", rtc_disable_command, "RTC disable"),   \
        SHELL_CMD("rtc_set_sec", "rss", rtc_set_sec_command, "RTC SetSec"),   \
        SHELL_CMD("rtc_reset", "rr", rtc_reset_command, "RTC reset"),

#ifdef __cplusplus
}
#endif

#endif /* RTC_COMMANDS_H */
