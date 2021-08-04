#ifndef FLASH_COMMANDS_H
#define FLASH_COMMANDS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

bool flash_scan_command(int32_t argc, char* argv[]);

#define FLASH_COMMANDS SHELL_CMD("flash_scan", "fs", flash_scan_command, "Flash scan"),

#ifdef __cplusplus
}
#endif

#endif /* FLASH_COMMANDS_H */
