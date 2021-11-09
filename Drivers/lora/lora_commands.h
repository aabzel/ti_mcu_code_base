#ifndef LORA_COMMANDS_H
#define LORA_COMMANDS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "cli_manager.h"

bool lora_diag_command(int32_t argc, char* argv[]);
bool lora_sent_queue_command(int32_t argc, char* argv[]);

#define LORA_COMMANDS \
    SHELL_CMD("lora_diag", "ld", lora_diag_command, "LoRa diag"), \
    SHELL_CMD("lora_queue", "lq", lora_sent_queue_command, "LoRa send queue"),

#ifdef __cplusplus
}
#endif

#endif /* LORA_COMMANDS_H */
