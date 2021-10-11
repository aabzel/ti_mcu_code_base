#ifndef TCAN4550_COMMANDS_H
#define TCAN4550_COMMANDS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "cli_manager.h"

bool tcan4550_diag_command(int32_t argc, char* argv[]);
bool tcan4550_init_command(int32_t argc, char* argv[]);
bool tcan4550_int_diag_command(int32_t argc, char* argv[]);
bool tcan4550_read_reg_command(int32_t argc, char* argv[]);
bool tcan4550_reg_map_command(int32_t argc, char* argv[]);
bool tcan4550_reset_command(int32_t argc, char* argv[]);
bool tcan4550_write_reg_command(int32_t argc, char* argv[]);

#define TCAN4550_COMMANDS                                                                                                \
        SHELL_CMD("tcan4550_diag", "cad", tcan4550_diag_command, "TCAN4550 diag"),                                       \
        SHELL_CMD("tcan4550_init", "ci", tcan4550_init_command, "TCAN4550 init"),                                       \
        SHELL_CMD("tcan4550_reg_map", "cm", tcan4550_reg_map_command, "TCAN4550 reg map"),                              \
        SHELL_CMD("tcan4550_reg_rd", "crr", tcan4550_read_reg_command, "TCAN4550 read reg"),                            \
        SHELL_CMD("tcan4550_reset", "crt", tcan4550_reset_command, "TCAN4550 reset"),                                   \
        SHELL_CMD("tcan4550_reg_wr", "cwr", tcan4550_write_reg_command, "TCAN4550 write reg"),                          \


#ifdef __cplusplus
}
#endif

#endif /* TCAN4550_COMMANDS_H  */
