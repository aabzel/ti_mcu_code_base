/**
 *  Description : manager for Bash-like UART console.
 *  Command Line Interface (CLI)
 */
#include "cli_manager.h"

#include <ctype.h>

#include "base_cmd.h"
#include "data_utils.h"
#include "io_utils.h"
#include "log_commands.h"
#include "none_blocking_pause.h"
#include "cli_commands.h"
#include "str_utils.h"
#include "sys_config.h"
#include "table_utils.h"
#include "uart_drv.h"
#include "uart_string_reader.h"
#include "writer_generic.h"

bool cli_echo = true;
uint32_t cli_task_cnt = 0;
bool cli_init_done = false;
static const shell_cmd_info_t shell_commands[] = {SHELL_COMMANDS COMMANDS_END};

/*logic AND for keyWords */
static bool is_print_cmd(const shell_cmd_info_t* const cmd, const char* const sub_name1, const char* const sub_name2) {
    bool res = false;
    if(NULL != cmd) {
        if((NULL == sub_name1) && (NULL == sub_name2)) {
            res = true;
        } else if((NULL != sub_name1) && (NULL == sub_name2)) {
            /*one subname done*/
            res = false;
            if(NULL != cmd->short_name) {
                if(NULL != str_case_str(cmd->short_name, sub_name1)) {
                    res = true;
                }
            }
            if(NULL != cmd->long_name) {
                if(NULL != str_case_str(cmd->long_name, sub_name1)) {
                    res = true;
                }
            }
            if(NULL != cmd->description) {
                if(NULL != str_case_str(cmd->description, sub_name1)) {
                    res = true;
                }
            }
        } else if((NULL != sub_name1) && (NULL != sub_name2)) {
            /*two subnames done*/
            res = false;
            if(NULL != cmd->short_name) {
                if(NULL != str_case_str(cmd->short_name, sub_name1)) {
                    if(NULL != str_case_str(cmd->short_name, sub_name2)) {
                        res = true;
                    }
                }
            }
            if(NULL != cmd->long_name) {
                if(NULL != str_case_str(cmd->long_name, sub_name1)) {
                    if(NULL != str_case_str(cmd->long_name, sub_name2)) {
                        res = true;
                    }
                }
            }
            if(NULL != cmd->description) {
                if(NULL != str_case_str(cmd->description, sub_name1)) {
                    if(NULL != str_case_str(cmd->description, sub_name2)) {
                        res = true;
                    }
                }
            }
        }
    }

    return res;
}

bool cli_init(void) {
    bool res = false;
    if(false == uart_string_reader_init(&cmd_reader)) {
        cli_init_done = false;
    } else {
        cli_set_echo(true);
        cli_init_done = true;
        res = true;
    }
    return res;
}


bool cli_process(void) {
    if(true == huart[CLI_UART_NUM].rx_int) {
        huart[CLI_UART_NUM].rx_int = false;
#ifndef USE_HAL_DRIVER
        uart_string_reader_rx_callback(&cmd_reader, (char)huart[CLI_UART_NUM].rx_byte);
        //uart_read(CLI_UART_NUM, &huart[CLI_UART_NUM].rx_byte, 1);
#endif /*USE_HAL_DRIVER*/
    }

    bool res = false;
    static bool entry = false;
    if(false == entry) {
        /* recursive protection from test which call during execution */
        if(true == cli_init_done) {
            entry = true;
            cli_task_cnt++;
            if(true == huart[CLI_UART_NUM].tx_int) {
                dbg_o.f_transmit(&dbg_o);
                huart[CLI_UART_NUM].tx_int = false;
            }
            uart_string_reader_proccess(&cmd_reader);
            res = true;
            entry = false;
        }
    }
    return res;
}


void *cliThread(void *arg0){
    while (1) {
       // cli_process();
    }
}

bool process_shell_cmd(char* cmd_line) {
#ifdef HAS_CLI_DEBUG
    io_printf("proc command [%s]" CRLF, cmd_line);
#endif /*HAS_CLI_DEBUG*/
    static int shell_argc = 0;
    static char* shell_argv[SHELL_MAX_ARG_COUNT];
    char* p = cmd_line;
    const shell_cmd_info_t* cmd = shell_commands;

    shell_argc = 0;
    memset(shell_argv, 0, sizeof(shell_argv));
    while((shell_argc < SHELL_MAX_ARG_COUNT) && (*p != 0)) {
        while(isspace((uint8_t)*p)) {
            p++;
        }
        if(*p != '\0') {
            shell_argv[shell_argc] = p;
            shell_argc++;
            while(*p && !isspace((uint8_t)*p)) {
                p++;
            }
            if(*p != '\0') {
                *p = '\0';
                p++;
            }
        }
    }
    if(shell_argc == 0) {
        shell_prompt();
        return true;
    }
    while(cmd->handler) {
        if((cmd->long_name && __strcasecmp(cmd->long_name, shell_argv[0]) == 0) ||
           (cmd->short_name && __strcasecmp(cmd->short_name, shell_argv[0]) == 0)) {
            cmd->handler(shell_argc - 1, shell_argv + 1);
            shell_prompt();
            return true;
        }
        cmd++;
    }
    if(user_mode) {
        LOG_ERROR(SYS, "Unknown command [%s]", shell_argv[0]);
    } else {
        dump_cmd_result_ex(false, "Unknown command");
    }
    shell_prompt();
    return false;
}

void help_dump_key(const char* sub_name1, const char* sub_name2) {
    uint16_t num=1;
    const shell_cmd_info_t* cmd = shell_commands;
    io_printf("Available commands:");
    if(sub_name1) {
        io_printf("Key1:%s" CRLF, sub_name1);
    }
    if(sub_name2) {
        io_printf("Key2:%s" CRLF, sub_name2);
    }
    io_putstr(CRLF);
    static const table_col_t cols[] = {{5, "num"}, {10, "short"}, {20, "long command"}, {13, "Description"}};
    table_header(&dbg_o.s, cols, ARRAY_SIZE(cols));
    while(cmd->handler) {
        if(is_print_cmd(cmd, sub_name1, sub_name2)) {
            io_printf( TSEP );
            io_printf(" %3u " TSEP, num);
            io_printf(" %8s " TSEP, cmd->short_name ? cmd->short_name : "");
            io_printf(" %18s " TSEP, cmd->long_name ? cmd->long_name : "");
            io_printf(" %s ", cmd->description ? cmd->description : "");
            io_printf(CRLF);
#ifdef NORTOS
            wait_in_loop_ms(2);
#endif /*NORTOS*/
            num++;
        }
        cmd++;
    }
    table_row_bottom(&dbg_o.s, cols, ARRAY_SIZE(cols));
}

bool cli_set_echo(bool echo_val) {
    cli_echo = echo_val;
    return true;
}

bool cli_get_echo(void) {
    return cli_echo;
}

bool cli_toggle_echo(void) {
    cli_echo = !cli_echo;
    return true;
}
