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
#include "shell_commands.h"
#include "str_utils.h"
#include "table_utils.h"
#include "uart_drv.h"
#include "uart_string_reader.h"
#include "writer_generic.h"

uint32_t cli_task_cnt = 0;
bool cli_init_done = false;
static const shell_cmd_info_t shell_commands[] = {SHELL_COMMANDS COMMANDS_END};

/*logic AND for keyWords */
static bool is_print_cmd(const shell_cmd_info_t* const cmd, const char* const subName1, const char* const subName2) {
    bool res = false;
    if(NULL != cmd) {
        if((NULL == subName1) && (NULL == subName2)) {
            res = true;
        } else if((NULL != subName1) && (NULL == subName2)) {
            /*one subname done*/
            res = false;
            if(NULL != cmd->short_name) {
                if(NULL != str_case_str(cmd->short_name, subName1)) {
                    res = true;
                }
            }
            if(NULL != cmd->long_name) {
                if(NULL != str_case_str(cmd->long_name, subName1)) {
                    res = true;
                }
            }
            if(NULL != cmd->description) {
                if(NULL != str_case_str(cmd->description, subName1)) {
                    res = true;
                }
            }
        } else if((NULL != subName1) && (NULL != subName2)) {
            /*two subnames done*/
            res = false;
            if(NULL != cmd->short_name) {
                if(NULL != str_case_str(cmd->short_name, subName1)) {
                    if(NULL != str_case_str(cmd->short_name, subName2)) {
                        res = true;
                    }
                }
            }
            if(NULL != cmd->long_name) {
                if(NULL != str_case_str(cmd->long_name, subName1)) {
                    if(NULL != str_case_str(cmd->long_name, subName2)) {
                        res = true;
                    }
                }
            }
            if(NULL != cmd->description) {
                if(NULL != str_case_str(cmd->description, subName1)) {
                    if(NULL != str_case_str(cmd->description, subName2)) {
                        res = true;
                    }
                }
            }
        }
    }

    return res;
}

void cli_init(void) {
    if(false == uart_string_reader_init(&cmd_reader)) {
        cli_init_done = false;
    } else {
        set_echo(true);
        cli_init_done = true;
    }
}

bool cli_process(void) {
    if(true == huart[0].rx_int) {
        uart_string_reader_rx_callback(&cmd_reader, (char)huart[0].rx_byte);
        huart[0].rx_int = false;
        UART_read(huart[0].uart_h, &huart[0].rx_byte, 1);
    }

    bool res = false;
    static bool entry = false;
    if(false == entry) {
        /* recursive protection from test which call during execution */
        if(true == cli_init_done) {
            entry = true;
            cli_task_cnt++;
            if(true == huart[0].tx_int) {
                dbg_o.f_transmit(&dbg_o);
                huart[0].tx_int = false;
            }
            uart_string_reader_proccess(&cmd_reader);
            res = true;
            entry = false;
        }
    }
    return res;
}

void process_shell_cmd(char* cmd_line) {
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
        return;
    }
    while(cmd->handler) {
        if((cmd->long_name && __strcasecmp(cmd->long_name, shell_argv[0]) == 0) ||
           (cmd->short_name && __strcasecmp(cmd->short_name, shell_argv[0]) == 0)) {
            cmd->handler(shell_argc - 1, shell_argv + 1);
            shell_prompt();
            return;
        }
        cmd++;
    }
    if(user_mode) {
        LOG_ERROR(SYS, "Unknown command [%s]", shell_argv[0]);
    } else {
        dump_cmd_result_ex(false, "Unknown command");
    }
    shell_prompt();
}

void help_dump_key(const char* subName1, const char* subName2) {
    const shell_cmd_info_t* cmd = shell_commands;
    io_printf("Available commands:");
    if(subName1) {
        io_printf("Key1:%s" CRLF, subName1);
    }
    if(subName2) {
        io_printf("Key2:%s" CRLF, subName2);
    }
    io_putstr(CRLF);
    static const table_col_t cols[] = {
        {10, "short"}, {20, "long command"}, {13, "Description"}
    };
    table_header(&dbg_o.s, cols, ARRAY_SIZE(cols));
    while(cmd->handler) {
        if(is_print_cmd(cmd, subName1, subName2)) {
            io_printf(TSEP" %8s "TSEP , cmd->short_name ? cmd->short_name : "");
            io_printf(" %18s " TSEP , cmd->long_name ? cmd->long_name : "");
            io_printf(" %s " , cmd->description ? cmd->description : "");
            io_printf(CRLF);
        }
        wait_in_loop_ms(4);

        cmd++;
    }
    table_row_bottom(&dbg_o.s, cols, ARRAY_SIZE(cols));
}
