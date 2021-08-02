#ifndef SPI_COMMANDS_H
#define SPI_COMMANDS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "ostream.h"

bool spi_send_command(int32_t argc, char* argv[]);
bool spi_diag_command(int32_t argc, char* argv[]);

#define SPI_COMMANDS                                                                  \
        SHELL_CMD("spi_diag", "sd", spi_diag_command, "SPI diag"),                    \
        SHELL_CMD("spi_send", "ss", spi_send_command, "SPI send hex str"),

#ifdef __cplusplus
}
#endif

#endif /* SPI_COMMANDS_H */
