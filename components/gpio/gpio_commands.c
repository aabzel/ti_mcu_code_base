#include "gpio_commands.h"

#include <inttypes.h>
#include <stdio.h>

#include "convert.h"
#include "ctype.h"
#include "data_utils.h"
#include "diag_page_nums.h"
#include "diag_report.h"
#include "gpio_drv.h"
#include "io_utils.h"
#include "log.h"
#include "ostream.h"
#include "shell.h"
#include "str_utils.h"
#include "table_utils.h"

/*
gg D 1
*/
bool gpio_get_command(int32_t argc, char* argv[]) {
    bool res = false;
    if (2 == argc) {
        res = true;
        uint8_t logic_level = 0xFF;
        uint8_t port_pin_num = 0;
        char port_pin_char = '0';

        port_pin_char = *(argv[0]);

        if (true == res) {
            res = try_str2uint8(argv[1], &port_pin_num);
            if (false == res) {
                LOG_ERROR(SYS, "Unable to extract port_pin_num %s", argv[1]);
            }
        }

        if (true == res) {
            res = gpio_get_state(port_pin_char, port_pin_num, &logic_level);
            if (false == res) {
                LOG_ERROR(SYS, "Unable to get gpio state");
            } else {
                io_printf("%u" CRLF, logic_level);
            }
        }
    } else {
        LOG_ERROR(SYS, "Usage: gg gpio_port gpio_pin");
        LOG_INFO(SYS, "gpio_port [A...Z]");
        LOG_INFO(SYS, "gpio_pin 0....15 ");
    }
    return res;
}

bool gpio_set_command(int32_t argc, char* argv[]) {
    bool res = false;
    if (3 == argc) {
        res = true;
        uint8_t logic_level = 0xFF;
        uint8_t port_pin_num = 0;
        char port_pin_char = '0';

        port_pin_char = *(argv[0]);

        if (true == res) {
            res = try_str2uint8(argv[1], &port_pin_num);
            if (false == res) {
                LOG_ERROR(SYS, "Unable to extract port_pin_num %s", argv[1]);
            }
        }

        if (true == res) {
            res = try_str2uint8(argv[2], &logic_level);
            if (false == res) {
                LOG_ERROR(SYS, "Unable to extract logic_level %s", argv[2]);
            }
        }

        if (true == res) {
            res = gpio_set_state(port_pin_char, port_pin_num, logic_level);
            if (false == res) {
                LOG_ERROR(SYS, "Unable to set gpio state");
            } else {
                io_printf("%u" CRLF, logic_level);
            }
        }
    } else {
        LOG_ERROR(SYS, "Usage: gs gpio_port gpio_pin logic_level");
        LOG_INFO(SYS, "gpio_port [A...Z]");
        LOG_INFO(SYS, "gpio_pin 0....15 ");
        LOG_INFO(SYS, "logic_level 0..1 ");
    }
    return res;
}

static bool is_print_gpio(char port, uint8_t pin, char* key_word1, char* key_word2) {
    bool res = false;

    if ((0 == strcmp("", key_word1)) && (0 == strcmp("", key_word2))) {
        res = true;
    } else {
        char temp_str[30] = "";
        snprintf(temp_str, sizeof(temp_str), "%c %u %s %s %s AF%u", port, pin, get_gpio_mode(port, pin),
                 get_gpio_pull_mode(port, pin), get_gpio_type(port, pin), get_gpio_alter_fun(port, pin));

        if (0 != strcmp("", key_word1) && 0 != strcmp("", key_word2)) {
            if (str_case_str(temp_str, key_word1) && str_case_str(temp_str, key_word2)) {
                res = true;
            }
        } else if (0 != strcmp("", key_word1) && 0 == strcmp("", key_word2)) {
            if (str_case_str(temp_str, key_word1)) {
                res = true;
            }
        } else {
            res = true;
        }
    }

    return res;
}

static bool diag_gpio(char* key_word1, char* key_word2) {
    bool res = false;

    static const table_col_t cols[] = {{5, "No"},   {5, "pin"},  {9, "GPIO"},  {7, "level"},
                                       {6, "mode"}, {6, "pull"}, {13, "type"}, {8, "AltFun"}};
    uint16_t num = 0;
    table_header(&dbg_o.s, cols, ARRAY_SIZE(cols));
    uint8_t logic_level = 0xFF;
    char port;
    for (port = 'a'; port <= 'i'; port++) {
        uint8_t pin=0;
        for ( pin = 0; pin <= 15; pin++) {
            res = gpio_get_state(port, pin, &logic_level);
            if (true == res) {
                if (is_print_gpio(port, pin, key_word1, key_word2)) {
                    io_printf(TSEP " %3u ", num);
                    io_printf(TSEP " %3u ", get_mcu_pin(port, pin));
                    io_printf(TSEP " GPIO%c", toupper(port));
                    io_printf("%02u " TSEP, pin);
                    io_printf("  %1u    " TSEP, logic_level);
                    io_printf(" %3s  " TSEP, get_gpio_mode(port, pin));
                    io_printf(" %4s " TSEP, get_gpio_pull_mode(port, pin));
                    io_printf(" %11s " TSEP, get_gpio_type(port, pin));
                    io_printf(" AF%02u   " TSEP, get_gpio_alter_fun(port, pin));
                    io_printf(CRLF);
                    num++;
                }
            }
        }
    }
    table_row_bottom(&dbg_o.s, cols, ARRAY_SIZE(cols));

    return res;
}

bool gpio_list_command(int32_t argc, char* argv[]) {
    bool res = false;
    char keyWord1[20] = "";
    char keyWord2[20] = "";
    if (1 == argc) {
        strncpy(keyWord1, argv[0], sizeof(keyWord1));
        res = true;
    } else if (0 == argc) {
        strncpy(keyWord1, "", sizeof(keyWord1));
        strncpy(keyWord2, "", sizeof(keyWord2));
        res = true;
    } else if (2 == argc) {
        strncpy(keyWord1, argv[0], sizeof(keyWord1));
        strncpy(keyWord2, argv[1], sizeof(keyWord2));
        res = true;
    } else {
        LOG_ERROR(SYS, "Usage: gl keyWord");
    }
    if (true == res) {
        res = diag_gpio(keyWord1, keyWord2);
    }
    return res;
}

bool gpio_toggle_command(int32_t argc, char* argv[]){
    bool res = false;
    if (2 == argc) {
        res = true;
        uint8_t port_pin_num = 0;
        char port_pin_char = '0';

        port_pin_char = *(argv[0]);

        if (true == res) {
            res = try_str2uint8(argv[1], &port_pin_num);
            if (false == res) {
                LOG_ERROR(SYS, "Unable to extract port_pin_num %s", argv[1]);
            }
        }

        if (true == res) {
            res = gpio_toggle(port_pin_char, port_pin_num);
            if (false == res) {
                LOG_ERROR(SYS, "Unable to toggle gpio");
            } else {
                io_printf("pin %u toggled" CRLF, port_pin_num);
            }
        }
    } else {
        LOG_ERROR(SYS, "Usage: gt gpio_port gpio_pin");
        LOG_INFO(SYS, "gpio_port [A...Z]");
        LOG_INFO(SYS, "gpio_pin 0....15 ");
    }
    return res;
}
