#include "gpio_commands.h"


#include <inttypes.h>
#include <stdio.h>

#include "base_cmd.h"
#include "convert.h"
#include "ctype.h"
#include "data_utils.h"
#include "gpio_diag.h"
#include "gpio_drv.h"
#include "io_utils.h"
#include "log.h"
#include "ostream.h"
#include "str_utils.h"
#include "table_utils.h"
#include "writer_config.h"
#include "board_layout.h"

/*
gg D 1
*/
bool gpio_get_command(int32_t argc, char* argv[]) {
    bool res = false;

    uint8_t logic_level = 0xFF;
    uint8_t pin_num=0xFF;
    char port_lett='_';

    if(1<=argc){
        port_lett=argv[0][0];
    }
    if(2<=argc){
            res = try_str2uint8(argv[1], &pin_num);
            if(false == res) {
                LOG_ERROR(SYS, "Unable to extract pin_num %s", argv[1]);
            }
    }

    if(true == res) {
        Pad_t pad;
        pad.port = PortLetter2PortNum(port_lett);
        pad.pin = pin_num;
        res = gpio_get_state(pad.byte, &logic_level);
        if(false == res) {
            LOG_ERROR(SYS, "Unable to get gpio state");
        } else {
            io_printf("%u" CRLF, logic_level);
        }
    }
    if(2!=argc) {
        LOG_ERROR(SYS, "Usage: gg port pin");
        LOG_INFO(SYS, "port [A...Z]");
        LOG_INFO(SYS, "pin 0....15 ");
    }
    return res;
}

bool gpio_set_command(int32_t argc, char* argv[]) {
    bool res = false;
    uint8_t pin_num=0xFF;
    char port_lett='_';
    uint8_t logic_level = 0xFF;
    if(1<=argc){
        port_lett=argv[0][0];
    }
    if(2<=argc){
            res = try_str2uint8(argv[1], &pin_num);
            if(false == res) {
                LOG_ERROR(SYS, "Unable to extract pin_num %s", argv[1]);
            }
    }

    if(3<=argc) {
            res = try_str2uint8(argv[2], &logic_level);
            if(false == res) {
                LOG_ERROR(SYS, "Unable to extract logic_level %s", argv[2]);
            }
    }
    if(argc<3) {
        LOG_ERROR(SYS, "Usage: gs port pin level");
        LOG_INFO(SYS, "port A....H");
        LOG_INFO(SYS, "pin 0....15 ");
        LOG_INFO(SYS, "level 0..1 ");
        res = false;
    }
    if(res) {
        Pad_t pad;
        pad.port = PortLetter2PortNum(port_lett);
        pad.pin = pin_num;
        res = gpio_set_state(pad.byte, logic_level);
        if(false == res) {
            LOG_ERROR(SYS, "Unable to set gpio state");
        } else {
            io_printf("%u" CRLF, logic_level);
        }
    }
    return res;
}

static bool diag_gpio(char* key_word1, char* key_word2) {
    bool res = false;

    static const table_col_t cols[] = {{5, "No"},
	{5, "gpioPort"},
	{5, "gpioPin"},
	{5, "mcuPin"},
    {7, "level"},
	{5, "dir"},
	{6, "edge"},
	{6, "pull"},
    {10, "AltFun"},
	{12, "name"}};
    uint16_t num = 0;
    table_header(&(curWriterPtr->s), cols, ARRAY_SIZE(cols));
    uint8_t logic_level = 0xFF;
    uint8_t i = 0;
    char temp_str[120];
    for(i = 0; i < gpio_get_cnt(); i++) {
        strcpy(temp_str, TSEP);
        gpio_get_state(PinTable[i].pad.byte, &logic_level);
        snprintf(temp_str, sizeof(temp_str), "%s  %s  " TSEP, temp_str, GpioPort2str(PinTable[i].pad.port));
        snprintf(temp_str, sizeof(temp_str), "%s %2u  " TSEP, temp_str, PinTable[i].pad.pin);
        snprintf(temp_str, sizeof(temp_str), "%s %3u " TSEP, temp_str, PinTable[i].mcu_pin);
        snprintf(temp_str, sizeof(temp_str), "%s   %s   " TSEP, temp_str, (1==logic_level) ? "H" : "L");

        if(is_contain(temp_str, key_word1, key_word2)) {
            io_printf(TSEP " %3u ", num);
            io_printf("%s\r\n", temp_str);
            num++;
        }
    }

    table_row_bottom(&(curWriterPtr->s), cols, ARRAY_SIZE(cols));

    return res;
}

bool gpio_list_command(int32_t argc, char* argv[]) {
    bool res = false;
    char keyWord1[20] = "";
    char keyWord2[20] = "";
    if(0 <= argc) {
        strncpy(keyWord1, "", sizeof(keyWord1));
        strncpy(keyWord2, "", sizeof(keyWord2));
        res = true;
    }
    if(1 <= argc) {
        strncpy(keyWord1, argv[0], sizeof(keyWord1));
        res = true;
    }
    if(2 <= argc) {
        strncpy(keyWord2, argv[1], sizeof(keyWord2));
        res = true;
    }

    if(2 < argc) {
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
