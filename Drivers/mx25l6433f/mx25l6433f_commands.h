#ifndef MX25L6433F_COMMANDS_H
#define MX25L6433F_COMMANDS_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

bool mx25_read_command(int32_t argc, char* argv[]);
bool mx25_write_command(int32_t argc, char* argv[]);
bool mx25_erase_command(int32_t argc, char* argv[]);
bool cmd_mx25_diag(int32_t argc, char* argv[]);

#define MX25L6433_COMMANDS                                                                              \
    SHELL_CMD("mx25_diag", "mxd", cmd_mx25_diag, "SPI NOR Flash diag"),                                 \  
    SHELL_CMD("mx25_read", "mxr", mx25l6435e_read_command, "Read SPI NOR flash"),                       \
    SHELL_CMD("mx25_write", "mxw", mx25l6435e_write_command,  "write SPI NOR flash"),                   \
    SHELL_CMD("mx25_errase", "mxe", mx25l6435e_erase_command, "Erase sector SPI NOR flash"),
    
    
#ifdef __cplusplus
}
#endif

#endif /* MX25L6433F_COMMANDS_H */
