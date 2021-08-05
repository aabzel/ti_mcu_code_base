#ifndef DAC_COMMANDS_H
#define DAC_COMMANDS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "ostream.h"

bool dac_init_command(int32_t argc, char* argv[]);
bool dac_diag_command(int32_t argc, char* argv[]);
bool dac_set_vol_command(int32_t argc, char* argv[]);
bool dac_set_code_command(int32_t argc, char* argv[]);
bool dac_set_mux_command(int32_t argc, char* argv[]);

#define DAC_COMMANDS                                                                                                   \
    SHELL_CMD("dac_init", "dai", dac_init_command, "DAC init"),                                                        \
        SHELL_CMD("dac_set_vol", "dav", dac_set_vol_command, "DAC set voltage"),                                       \
        SHELL_CMD("dac_diag", "dad", dac_diag_command, "DAC diag"),                                                    \
        SHELL_CMD("dac_set_mux", "dsm", dac_set_mux_command, "DAC set mux"),                                           \
        SHELL_CMD("dac_set_code", "dsc", dac_set_code_command, "DAC set code"),

#ifdef __cplusplus
}
#endif

#endif /* DAC_COMMANDS_H */
