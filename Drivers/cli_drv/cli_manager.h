/**
 * Description        : driver for Bash like UART concole.
 * command line interface (CLI)
 */

#ifndef CLI_DRIVER_H
#define CLI_DRIVER_H

#include <stdbool.h>
#include <stdint.h>

#include "uart_drv.h"

#define CLI_MANAGER_NAME "CLI"
#define CLI_PERIOD_MS 300U
#define CLI_UART CONFIG_UART_0

extern uint32_t cli_task_cnt;
extern bool cli_init_done;

#define SHELL_CMD(long_cmd, short_cmd, func, description)                                                              \
    { short_cmd, long_cmd, description, func }

bool cli_init(void);
bool cli_process(void);
void process_shell_cmd(char* cmd_line);
void help_dump_key(const char* subName1, const char* subName2);
void *cliThread(void *arg0);
#endif /*CLI_DRIVER_H*/
