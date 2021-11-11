/**
 *  Description : manager for Bash-like UART console.
 *  Command Line Interface (CLI)
 */
#include "cli_manager.h"

#include <ctype.h>

#include "data_utils.h"
#ifndef X86_64
#include "base_cmd.h"
#include "io_utils.h"
#include "log_commands.h"
#include "none_blocking_pause.h"
#include "cli_commands.h"
#include "table_utils.h"
#include "sys_config.h"
#include "uart_drv.h"
#include "uart_string_reader.h"
#include "writer_generic.h"
#include "writer_config.h"
#endif

#include "str_utils.h"
#include "uart_common.h"

bool cli_echo = true;
uint32_t cli_task_cnt = 0;
bool cli_init_done = false;
#ifndef X86_64
static const shell_cmd_info_t shell_commands[] = {SHELL_COMMANDS COMMANDS_END};
#endif

uint32_t cli_cmd_len_max = 0;

#ifdef HAS_CLI_CMD_HISTORY
char prev_cmd[40]="";
#endif

#ifndef X86_64
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
#endif

#ifndef X86_64
bool cli_init(void) {
    bool res = false;
    if(false == uart_string_reader_init(&cmd_reader)) {
        cli_init_done = false;
    } else {
#ifdef HAS_CLI_CMD_HISTORY
        memset(prev_cmd, 0x00,sizeof(prev_cmd));
#endif
        cli_cmd_len_max = 0;
        cli_set_echo(true);
        res = writer_init();
        cli_init_done = true;
        res = true;
    }
    return res;
}
#endif


#ifndef X86_64
bool cli_process(void) {
//    static uint8_t rx_byte=0;
    if(true == huart[UART_NUM_CLI].rx_int) {
        huart[UART_NUM_CLI].rx_int = false;
        huart[UART_NUM_CLI].rx_it_proc_done = true;
    }

    bool res = false;
    //static bool entry = false;
    //if(false == entry) {
        /* recursive protection from test which call during execution */
        if(true == cli_init_done) {
         //   entry = true;
            cli_task_cnt++;
            if(true == huart[UART_NUM_CLI].tx_int) {
                dbg_o.f_transmit(&dbg_o);
                huart[UART_NUM_CLI].tx_int = false;
            }
            uart_string_reader_proccess(&cmd_reader);
            res = true;
          //  entry = false;
        }
    //}
    return res;
}
#endif


/*
 cmd_line must be in RAM
 TODO: make cmd_line unchangeable
 */
bool cli_parse_args(char* cmd_line, int *argc, char** argv){
    bool res = false;
    int argc_loc=0;
    char* pRun = cmd_line;
    while((argc_loc < SHELL_MAX_ARG_COUNT) && (0x00!=*pRun)) {
        while(isspace((int)*pRun)) {
            pRun++;
        }
        if('\0' !=*pRun ) {
            argv[argc_loc] =( char*) pRun;
            argc_loc++;
            res = true;
            while(*pRun && !isspace((int)*pRun)) {
                pRun++;
            }
            if(*pRun != '\0') {
                *pRun = '\0';
                pRun++;
            }
        }
    }/*while*/
    (*argc)=argc_loc;

    return res;
}

#ifndef X86_64
bool process_shell_cmd(char* cmd_line) {
    bool res = false;
#ifdef HAS_CLI_DEBUG
    io_printf("proc command [%s] %u" CRLF, cmd_line, strlen(cmd_line));
    cli_cmd_len_max = rx_min32u(cli_cmd_len_max ,strlen(cmd_line));
#endif /*HAS_CLI_DEBUG*/

#ifdef HAS_CLI_CMD_HISTORY
    memset(prev_cmd, 0x00, sizeof(prev_cmd));
    if (strlen(cmd_line) < sizeof(prev_cmd)) {
        memcpy(prev_cmd, cmd_line, strlen(cmd_line));
    }
#endif
    static int shell_argc = 0;
    static char* shell_argv[SHELL_MAX_ARG_COUNT];
    const shell_cmd_info_t* cmd = shell_commands;

    /*TODO: make a single function for argument parsing*/
    shell_argc = 0;
    memset(shell_argv, 0, sizeof(shell_argv));
    cli_parse_args(cmd_line,&shell_argc, &shell_argv[0]);

    if(0 == shell_argc ) {
        shell_prompt();
        res = true;
    }

    if(false==res){
        while(NULL != cmd->handler) {
            if((cmd->long_name && __strcasecmp(cmd->long_name, shell_argv[0]) == 0) ||
               (cmd->short_name && __strcasecmp(cmd->short_name, shell_argv[0]) == 0)) {
                res = cmd->handler(shell_argc - 1, shell_argv + 1);
                if(false==res){
                    LOG_ERROR(SYS, "cmd error");
                }
                shell_prompt();
                //res = true;
                break;

            }
            cmd++;
        }
    }
    if(false==res){
        if(user_mode) {
            LOG_ERROR(SYS, "Unknown command [%s]", shell_argv[0]);
        } else {
            dump_cmd_result_ex(false, "Unknown command");
        }
        shell_prompt();
#ifdef HAS_CLI_CMD_HISTORY
        memset(prev_cmd, 0x00, sizeof(prev_cmd));
#endif
    }
    return res;
}
#endif

#ifndef X86_64
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
    table_header(&(curWriterPtr->s), cols, ARRAY_SIZE(cols));
    while(cmd->handler) {
        if(is_print_cmd(cmd, sub_name1, sub_name2)) {
            io_printf( TSEP );
            io_printf(" %3u " TSEP, num);
            io_printf(" %8s " TSEP, cmd->short_name ? cmd->short_name : "");
            io_printf(" %18s " TSEP, cmd->long_name ? cmd->long_name : "");
            io_printf(" %s ", cmd->description ? cmd->description : "");
            io_printf(CRLF);
#ifdef NORTOS
            //wait_in_loop_ms(1);
#endif /*NORTOS*/
            num++;
        }
        cmd++;
    }
    table_row_bottom(&(curWriterPtr->s), cols, ARRAY_SIZE(cols));
}
#endif

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

#ifdef HAS_CLI_CMD_HISTORY
Arrow_t cli_arrows_parse(char cur_char){
    Arrow_t arrow = ARROW_UNDEF;
    static char prev_char=0;
    static char prev_prev_char=0;
    if((0x41==cur_char)&&
       (0x5B==prev_char)&&
       (0x1B==prev_prev_char)){
        arrow = ARROW_UP;
    }
    prev_prev_char=prev_char;
    prev_char=cur_char;
    return arrow;
}
#endif
