#ifndef EXT_RAM_EMUL_COMMANDS_H
#define EXT_RAM_EMUL_COMMANDS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "cli_manager.h"

bool ext_ram_read_command(int32_t argc, char* argv[]);
bool ext_ram_write_command(int32_t argc, char* argv[]);
bool ext_ram_erase_command(int32_t argc, char* argv[]);
bool ext_ram_diag_command(int32_t argc, char* argv[]);

#define EXT_RAM_EMUL_COMMANDS                                                                                          \
    SHELL_CMD("ext_ram_diag", "erd", ext_ram_diag_command, "Ext RAM diag"),                                            \
        SHELL_CMD("ext_ram_read", "err", ext_ram_read_command, "Read ext RAM"),                                        \
        SHELL_CMD("ext_ram_write", "erw", ext_ram_write_command, "Write ext RAM"),                                     \
        SHELL_CMD("ext_ram_errase", "ere", ext_ram_erase_command, "Erase ext RAM"),

#ifdef __cplusplus
}
#endif

#endif /* EXT_RAM_EMUL_COMMANDS_H */
