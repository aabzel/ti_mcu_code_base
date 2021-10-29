#ifndef TELEMATIC_COMMANDS_H
#define TELEMATIC_COMMANDS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "ostream.h"

bool ping_command(int32_t argc, char* argv[]);

#define TELEMATIC_COMMANDS                                                                                                   \
    SHELL_CMD("ping", "ping", ping_command, "ZED F9P diag"),


#ifdef __cplusplus
}
#endif

#endif /* TELEMATIC_COMMANDS_H */
