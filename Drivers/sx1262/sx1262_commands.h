#ifndef SX1262_COMMANDS_H
#define SX1262_COMMANDS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "cli_manager.h"

bool sx1262_tx_command(int32_t argc, char* argv[]);
bool sx1262_rx_command(int32_t argc, char* argv[]);
bool sx1262_reg_map_command(int32_t argc, char* argv[]);
bool sx1262_init_command(int32_t argc, char* argv[]);
bool sx1262_diag_command(int32_t argc, char* argv[]);
bool sx1262_read_reg_command(int32_t argc, char* argv[]);
bool sx1262_send_opcode_command(int32_t argc, char* argv[]);

#define SX1262_COMMANDS                                                                                                \
    SHELL_CMD("lora_diag", "sxd", sx1262_diag_command, "SX1262 diag"),                                                 \
        SHELL_CMD("lora_opcode", "sxo", sx1262_send_opcode_command, "SX1262 send opcode"),                             \
        SHELL_CMD("lora_tx", "sxt", sx1262_tx_command, "SX1262 transmit"),                                             \
        SHELL_CMD("lora_rx", "sxrx", sx1262_rx_command, "SX1262 receive"),                                             \
        SHELL_CMD("lora_read", "sxi", sx1262_init_command, "SX1262 init"),                                             \
        SHELL_CMD("lora_reg_map", "sxm", sx1262_reg_map_command, "SX1262 reg map"),                                    \
        SHELL_CMD("lora_reg_rd", "sxr", sx1262_read_reg_command, "SX1262 read reg"),

#ifdef __cplusplus
}
#endif

#endif /* SX1262_COMMANDS_H */
