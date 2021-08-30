#ifndef LORA_COMMANDS_H
#define LORA_COMMANDS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "cli_manager.h"

bool lora_sent_command(int32_t argc, char* argv[]);

#define LORA_COMMANDS SHELL_CMD("lora_cmd", "lc", lora_sent_command, "LoRa send cli command"),

#ifdef __cplusplus
}
#endif

#endif /* LORA_COMMANDS_H */
