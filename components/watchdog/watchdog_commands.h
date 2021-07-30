#ifndef WDT_COMMANDS_H
#define WDT_COMMANDS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

bool wdt_diag_command(int32_t argc, char* argv[]);
bool wdt_set_command(int32_t argc, char* argv[]);

#define WDT_COMMANDS                                                                                                   \
    SHELL_CMD("wd_diag", "wdd", wdt_diag_command, "Watchdog diag"),                                                    \
        SHELL_CMD("wd_set", "wds", wdt_set_command, "Watchdog set"),

#ifdef __cplusplus
}
#endif

#endif /* WDT_COMMANDS_H */
