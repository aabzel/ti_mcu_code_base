#ifndef SX1262_COMMANDS_H
#define SX1262_COMMANDS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "cli_manager.h"

bool sx1262_clear_err_command(int32_t argc, char* argv[]);
bool sx1262_clear_fifo_command(int32_t argc, char* argv[]);
bool sx1262_diag_command(int32_t argc, char* argv[]);
bool sx1262_init_command(int32_t argc, char* argv[]);
bool sx1262_int_diag_command(int32_t argc, char* argv[]);
bool sx1262_read_fifo_command(int32_t argc, char* argv[]);
bool sx1262_read_reg_command(int32_t argc, char* argv[]);
bool sx1262_read_rx_payload_command(int32_t argc, char* argv[]);
bool sx1262_reg_map_command(int32_t argc, char* argv[]);
bool sx1262_reset_command(int32_t argc, char* argv[]);
bool sx1262_rx_command(int32_t argc, char* argv[]);
bool sx1262_send_opcode_command(int32_t argc, char* argv[]);
bool sx1262_set_freq_command(int32_t argc, char* argv[]);
bool sx1262_set_irq_command(int32_t argc, char* argv[]);
bool sx1262_set_modulation_command(int32_t argc, char* argv[]);
bool sx1262_set_packet_param_command(int32_t argc, char* argv[]);
bool sx1262_set_sync_word_command(int32_t argc, char* argv[]);
bool sx1262_sleep_command(int32_t argc, char* argv[]);
bool sx1262_tx_command(int32_t argc, char* argv[]);
bool sx1262_write_reg_command(int32_t argc, char* argv[]);

#define SX1262_COMMANDS                                                                                                \
    SHELL_CMD("sx1262_clr_err", "sxce", sx1262_clear_err_command, "SX1262 clear errors"),                              \
        SHELL_CMD("sx1262_clr_fifo", "sxcf", sx1262_clear_fifo_command, "SX1262 clear FIFO"),                          \
        SHELL_CMD("sx1262_diag", "sxd", sx1262_diag_command, "SX1262 diag"),                                           \
        SHELL_CMD("sx1262_init", "sxi", sx1262_init_command, "SX1262 init"),                                           \
        SHELL_CMD("sx1262_fifo", "sxrf", sx1262_read_fifo_command, "SX1262 read FIFO"),                                \
        SHELL_CMD("sx1262_int_diag", "sxid", sx1262_int_diag_command, "SX1262 interrupts diag"),                       \
        SHELL_CMD("sx1262_mod", "sxmd", sx1262_set_modulation_command, "SX1262 modulation"),                           \
        SHELL_CMD("sx1262_opcode", "sxo", sx1262_send_opcode_command, "SX1262 send opcode"),                           \
        SHELL_CMD("sx1262_read_rx_data", "sxrp", sx1262_read_rx_payload_command, "SX1262 read rx data"),               \
        SHELL_CMD("sx1262_reg_map", "sxm", sx1262_reg_map_command, "SX1262 reg map"),                                  \
        SHELL_CMD("sx1262_reg_rd", "sxrr", sx1262_read_reg_command, "SX1262 read reg"),                                \
        SHELL_CMD("sx1262_reg_wr", "sxwr", sx1262_write_reg_command, "SX1262 write reg"),                              \
        SHELL_CMD("sx1262_reset", "sxrt", sx1262_reset_command, "SX1262 reset"),                                       \
        SHELL_CMD("sx1262_rx", "sxr", sx1262_rx_command, "SX1262 receive"),                                            \
        SHELL_CMD("sx1262_set_freq", "sxsf", sx1262_set_freq_command, "SX1262 set freq"),                              \
        SHELL_CMD("sx1262_set_irq", "sxir", sx1262_set_irq_command, "SX1262 set IRQ"),                                 \
        SHELL_CMD("sx1262_set_pack_param", "sxpp", sx1262_set_packet_param_command, "SX1262 set packet param"),        \
        SHELL_CMD("sx1262_sleep", "sxs", sx1262_sleep_command, "SX1262 sleep"),                                        \
        SHELL_CMD("sx1262_sync", "sxssw", sx1262_set_sync_word_command, "SX1262 set sync word"),                       \
        SHELL_CMD("sx1262_tx", "sxt", sx1262_tx_command, "SX1262 transmit"),

#ifdef __cplusplus
}
#endif

#endif /* SX1262_COMMANDS_H */
