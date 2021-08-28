#ifndef SX1262_COMMANDS_H
#define SX1262_COMMANDS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "cli_manager.h"

bool sx1262_reset_command(int32_t argc, char* argv[]);
bool sx1262_clear_err_command(int32_t argc, char* argv[]);
bool sx1262_clear_fifo_command(int32_t argc, char* argv[]);
bool sx1262_read_fifo_command(int32_t argc, char* argv[]);
bool sx1262_tx_command(int32_t argc, char* argv[]);
bool sx1262_rx_command(int32_t argc, char* argv[]);
bool sx1262_reg_map_command(int32_t argc, char* argv[]);
bool sx1262_init_command(int32_t argc, char* argv[]);
bool sx1262_set_freq_command(int32_t argc, char* argv[]);
bool sx1262_diag_command(int32_t argc, char* argv[]);
bool sx1262_int_diag_command(int32_t argc, char* argv[]);
bool sx1262_read_reg_command(int32_t argc, char* argv[]);
bool sx1262_send_opcode_command(int32_t argc, char* argv[]);
bool sx1262_sleep_command(int32_t argc, char* argv[]);

#define SX1262_COMMANDS                                                                                                \
    SHELL_CMD("lora_diag", "sxd", sx1262_diag_command, "SX1262 diag"),                                                 \
    SHELL_CMD("lora_sleep", "sxs", sx1262_sleep_command, "SX1262 sleep"),                                                 \
        SHELL_CMD("lora_int_diag", "sxid", sx1262_int_diag_command, "SX1262 interrupts diag"),                         \
        SHELL_CMD("lora_opcode", "sxo", sx1262_send_opcode_command, "SX1262 send opcode"),                             \
        SHELL_CMD("lora_fifo", "sxf", sx1262_read_fifo_command, "SX1262 read FIFO"),                                   \
        SHELL_CMD("lora_set_freq", "sxsf", sx1262_set_freq_command, "SX1262 set freq"),                                \
        SHELL_CMD("lora_clr_fifo", "sxcf", sx1262_clear_fifo_command, "SX1262 clear FIFO"),                            \
        SHELL_CMD("lora_clr_err", "sxce", sx1262_clear_err_command, "SX1262 clear errors"),                            \
        SHELL_CMD("lora_tx", "sxt", sx1262_tx_command, "SX1262 transmit"),                                             \
        SHELL_CMD("lora_rx", "sxr", sx1262_rx_command, "SX1262 receive"),                                              \
        SHELL_CMD("lora_reset", "sxrt", sx1262_reset_command, "SX1262 reset"),                                         \
        SHELL_CMD("lora_read", "sxi", sx1262_init_command, "SX1262 init"),                                             \
        SHELL_CMD("lora_reg_map", "sxm", sx1262_reg_map_command, "SX1262 reg map"),                                    \
        SHELL_CMD("lora_reg_rd", "sxrr", sx1262_read_reg_command, "SX1262 read reg"),

#ifdef __cplusplus
}
#endif

#endif /* SX1262_COMMANDS_H */
