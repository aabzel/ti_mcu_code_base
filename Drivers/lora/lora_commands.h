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

#define LORA_COMMANDS_BASE \
    SHELL_CMD("lora_diag", "ld", lora_diag_command, "LoRa diag"), \
    SHELL_CMD("lora_queue", "lq", lora_sent_queue_command, "LoRa send queue"),

#ifdef HAS_DEBUG
bool lora_horizon_command(int32_t argc, char* argv[]);

#define LORA_COMMANDS_DEBUG \
    SHELL_CMD("lora_mast", "lh", lora_horizon_command, "mast horizon"),
#else
#define LORA_COMMANDS_DEBUG
#endif

#define LORA_COMMANDS \
        LORA_COMMANDS_BASE \
        LORA_COMMANDS_DEBUG


#ifdef __cplusplus
}
#endif

#endif /* LORA_COMMANDS_H */
