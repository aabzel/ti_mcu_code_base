/**
 * Description        : driver for Bash like UART concole.
 * command line interface (CLI)
 */

#ifndef CLI_DRIVER_H
#define CLI_DRIVER_H

#include <stdbool.h>
#include <stdint.h>

#define CLI_MANAGER_NAME "CLI"
#define CLI_PERIOD_MS 300U

extern uint32_t cli_task_cnt;
extern bool cli_init_done;

void cli_init(void);
bool cli_process(void);

#endif /*CLI_DRIVER_H*/
