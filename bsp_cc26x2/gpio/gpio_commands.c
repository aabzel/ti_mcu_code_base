#include "gpio_commands.h"

#include <inttypes.h>
#include <stdio.h>

#include "base_cmd.h"
#include "convert.h"
#include "ctype.h"
#include "data_utils.h"
#include "diag_page_nums.h"
#include "diag_report.h"
#include "gpio_drv.h"
#include "io_utils.h"
#include "log.h"
#include "ostream.h"
#include "str_utils.h"
#include "table_utils.h"

/*
gg D 1
*/
bool gpio_get_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(1 == argc) {
        res = true;
        uint8_t logic_level = 0xFF;
        uint8_t port_pin_num = 0;

        if(true == res) {
            res = try_str2uint8(argv[0], &port_pin_num);
            if(false == res) {
                LOG_ERROR(SYS, "Unable to extract port_pin_num %s", argv[0]);
            }
        }

        if(true == res) {
            res = gpio_get_state(port_pin_num, &logic_level);
            if(false == res) {
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
    if(2 == argc) {
        res = true;
        uint8_t logic_level = 0xFF;
        uint8_t port_pin_num = 0;

        if(true == res) {
            res = try_str2uint8(argv[0], &port_pin_num);
            if(false == res) {
                LOG_ERROR(SYS, "Unable to extract port_pin_num %s", argv[0]);
            }
        }

        if(true == res) {
            res = try_str2uint8(argv[1], &logic_level);
            if(false == res) {
                LOG_ERROR(SYS, "Unable to extract logic_level %s", argv[1]);
            }
        }

        if(true == res) {
            res = gpio_set_state(port_pin_num, logic_level);
            if(false == res) {
                LOG_ERROR(SYS, "Unable to set gpio state");
            } else {
                io_printf("%u" CRLF, logic_level);
            }
        }
    } else {
        LOG_ERROR(SYS, "Usage: gs gpio_port gpio_pin logic_level");
        LOG_INFO(SYS, "gpio_pin 0....30 ");
        LOG_INFO(SYS, "logic_level 0..1 ");
    }
    return res;
}

static bool diag_gpio(char* key_word1, char* key_word2) {
    bool res = false;

    static const table_col_t cols[] = {{5, "No"},  {5, "dio"},  {5, "aux"},  {5, "pin"},   {5, "dir"},    {7, "level"},
                                       {5, "irq"}, {6, "edge"}, {6, "pull"}, {13, "type"}, {10, "AltFun"}};
    uint16_t num = 0;
    table_header(&dbg_o.s, cols, ARRAY_SIZE(cols));
    uint8_t logic_level = 0xFF;

    uint8_t io_pin = 0;
    char temp_str[120];
    for(io_pin = 0; io_pin <= ARRAY_SIZE(PinTable); io_pin++) {
        res = gpio_get_state(io_pin, &logic_level);
        if(true == res) {
            strcpy(temp_str, TSEP);
            snprintf(temp_str, sizeof(temp_str), "%s %3u " TSEP, temp_str, io_pin);
            snprintf(temp_str, sizeof(temp_str), "%s %3u " TSEP, temp_str, get_aux_num(io_pin));
            snprintf(temp_str, sizeof(temp_str), "%s %3u " TSEP, temp_str, get_mcu_pin(io_pin));
            snprintf(temp_str, sizeof(temp_str), "%s %2s  " TSEP, temp_str, get_pin_dir(io_pin));
            snprintf(temp_str, sizeof(temp_str), "%s  %s    " TSEP, temp_str, (1 == logic_level) ? "H" : "L");
            snprintf(temp_str, sizeof(temp_str), "%s  %s  " TSEP, temp_str, (1 == is_edge_irq_en(io_pin)) ? "Y" : "N");
            snprintf(temp_str, sizeof(temp_str), "%s %4s " TSEP, temp_str, get_gpio_edge(io_pin));
            snprintf(temp_str, sizeof(temp_str), "%s %4s " TSEP, temp_str, get_gpio_pull_mode(io_pin));
            snprintf(temp_str, sizeof(temp_str), "%s %11s " TSEP, temp_str, get_gpio_type(io_pin));
            snprintf(temp_str, sizeof(temp_str), "%s %8s " TSEP, temp_str, get_gpio_alter_fun(io_pin));
            snprintf(temp_str, sizeof(temp_str), "%s" CRLF, temp_str);

            if(is_contain(temp_str, key_word1, key_word2)) {
                io_printf(TSEP " %3u ", num);
                io_printf("%s", temp_str);
                num++;
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
    if(1 == argc) {
        strncpy(keyWord1, argv[0], sizeof(keyWord1));
        res = true;
    } else if(0 == argc) {
        strncpy(keyWord1, "", sizeof(keyWord1));
        strncpy(keyWord2, "", sizeof(keyWord2));
        res = true;
    } else if(2 == argc) {
        strncpy(keyWord1, argv[0], sizeof(keyWord1));
        strncpy(keyWord2, argv[1], sizeof(keyWord2));
        res = true;
    } else {
        LOG_ERROR(SYS, "Usage: gl keyWord");
    }
    if(true == res) {
        res = diag_gpio(keyWord1, keyWord2);
    }
    return res;
}

bool gpio_toggle_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(1 == argc) {
        res = true;
        uint8_t port_pin_num = 0;

        if(true == res) {
            res = try_str2uint8(argv[0], &port_pin_num);
            if(false == res) {
                LOG_ERROR(SYS, "Unable to extract port_pin_num %s", argv[0]);
            }
        }

        if(true == res) {
            res = gpio_toggle(port_pin_num);
            if(false == res) {
                LOG_ERROR(SYS, "Unable to toggle gpio");
            } else {
                io_printf("pin %u toggled" CRLF, port_pin_num);
            }
        }
    } else {
        LOG_ERROR(SYS, "Usage: gt gpio_port gpio_pin");
        LOG_INFO(SYS, "gpio_pin 0....15 ");
    }
    return res;
}