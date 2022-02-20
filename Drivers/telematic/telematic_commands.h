#ifndef TELEMATIC_COMMANDS_H
#define TELEMATIC_COMMANDS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "ostream.h"

#ifdef HAS_GNSS
bool link_info_command(int32_t argc, char* argv[]);

#define TELEMATIC__LINK_INFO_COMMANDS                                               \
    SHELL_CMD("lora_cmd", "tcm", telematic_sent_command, "CmdOnRemote"),
#else
#define TELEMATIC__LINK_INFO_COMMANDS
#endif

bool ping_command(int32_t argc, char* argv[]);
bool chat_command(int32_t argc, char* argv[]);
bool telematic_sent_command(int32_t argc, char* argv[]);

#define TELEMATIC_COMMANDS                                               \
    TELEMATIC__LINK_INFO_COMMANDS                                        \
    SHELL_CMD("lora_cmd", "tcm", telematic_sent_command, "CmdOnRemote"), \
    SHELL_CMD("ping", "ping", ping_command, "ping"),                     \
    SHELL_CMD("chat", "chat", chat_command, "chat"),

#ifdef __cplusplus
}
#endif

#endif /* TELEMATIC_COMMANDS_H */
