/**
 * Description        : driver for Bash like UART concole.
 * command line interface (CLI)
 */

#ifndef CLI_DRIVER_H
#define CLI_DRIVER_H

#include <stdbool.h>
#include <stdint.h>

#ifndef X86_64
#include "uart_drv.h"
#endif

#define CLI_MANAGER_NAME "CLI"
#define CLI_PERIOD_MS 300U
#define CLI_UART CONFIG_UART_0
#define SHELL_MAX_ARG_COUNT 12


extern uint32_t cli_task_cnt;
extern bool cli_init_done;
extern bool cli_echo;
extern bool cli_output;

typedef enum eArrow_t {
    ARROW_UP = 0,
    ARROW_DOWN = 1,
    ARROW_LEFT = 2,
    ARROW_RIGHT = 3,
    ARROW_UNDEF = 4,
} Arrow_t;

#define SHELL_CMD(long_cmd, short_cmd, func, description)                                                              \
    { short_cmd, long_cmd, description, func }

#ifdef HAS_CLI_CMD_HISTORY
extern char prev_cmd[40];
#endif

bool cli_init(void);
bool cli_process(void);
bool process_shell_cmd(char* cmd_line);
void help_dump_key(const char* subName1, const char* subName2);
void* cliThread(void* arg0);
bool cli_parse_args(char* cmd_line, int* argc, char** argv);
bool cli_set_echo(bool echo);
bool cli_get_echo(void);
Arrow_t cli_arrows_parse(char cur_char);
bool cli_set_echo(bool echo_val);
bool cli_get_echo(void);
bool cli_toggle_echo(void);
#ifdef HAS_TIRTOS
void cli_create_task(void);
#endif

#endif /*CLI_DRIVER_H*/
