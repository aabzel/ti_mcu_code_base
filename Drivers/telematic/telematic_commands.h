#ifndef TELEMATIC_COMMANDS_H
#define TELEMATIC_COMMANDS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "ostream.h"

bool ping_command(int32_t argc, char* argv[]);
bool chat_command(int32_t argc, char* argv[]);
bool lora_sent_command(int32_t argc, char* argv[]);

#define TELEMATIC_COMMANDS                           \
    SHELL_CMD("lora_cmd", "lc", lora_sent_command, "Cmd on remote"), \
    SHELL_CMD("ping", "ping", ping_command, "ping"), \
    SHELL_CMD("chat", "chat", chat_command, "chat"),


#ifdef __cplusplus
}
#endif

#endif /* TELEMATIC_COMMANDS_H */
