#ifndef HEALTH_MONITOR_COMMAND_H
#define HEALTH_MONITOR_COMMAND_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "base_cmd.h"

bool health_monitor_command(int32_t argc, char *argv[]);

#define HEALTH_MONITOR_COMMANDS                      \
  SHELL_CMD("health_mon", "hm", health_monitor_command, "Health monitor"),

#ifdef __cplusplus
}
#endif

#endif /* HEALTH_MONITOR_COMMAND_H */
