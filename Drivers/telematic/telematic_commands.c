#include "telematic_commands.h"

#include <inttypes.h>
#include <stdio.h>
#include <time.h>
#ifdef HAS_LOG
#include "debug_info.h"
#include "io_utils.h"
#include "log.h"
#endif
#include "convert.h"
#include "core_driver.h"
#ifdef HAS_LORA
#include "lora_drv.h"
#endif
#include "system.h"
#include "tbfp_protocol.h"
#ifdef HAS_ZED_F9P
#include "zed_f9p_drv.h"
#endif

bool telematic_sent_command(int32_t argc, char* argv[]) {
    bool res = false;
    uint8_t tx_array[256] = "";
    uint16_t tx_array_len = 0;
    uint32_t timeout_s = 0;
    uint8_t interface = IF_NONE;

    if(1 <= argc) {
        res = try_str2uint8(argv[0], &interface);
    }

    if(2 <= argc) {
        res = true;
        uint8_t a = 0;
        snprintf((char*)tx_array, sizeof(tx_array), "%s%s", tx_array, argv[1]);
        LOG_INFO(SYS, "arg: [%s]", (char*)tx_array);
        for(a = 2; a < argc; a++) {
            snprintf((char*)tx_array, sizeof(tx_array), "%s %s", tx_array, argv[a]);
        }
        LOG_INFO(SYS, "arg: [%s]", (char*)tx_array);
        tx_array_len = strlen((char*)tx_array) + 1;
    }

    if(0 == argc) {
        LOG_ERROR(SYS, "Usage: lcm if cmd arg1 arg2 [argN]");
        LOG_INFO(SYS, "cli_cmd command for remote target");
        res = false;
    }

    if(res) {
        res = false;
        (void)tx_array_len;
        (void)timeout_s;
        res = tbfp_send_cmd(tx_array, tx_array_len, (Interfaces_t)interface);
        if(res) {
            LOG_INFO(SYS, "OK");
        }
    }

    return res;
}

bool ping_command(int32_t argc, char* argv[]) {
    bool res = false;
    uint8_t interface = IF_NONE;
    if(0 <= argc) {
        res = true;
        interface = IF_LORA;
    }

    if(1 <= argc) {
        res = try_str2uint8(argv[0], &interface);
    }

    if(res) {
        res = tbfp_send_ping(FRAME_ID_PING, (Interfaces_t)interface);
        if(res) {
            LOG_INFO(SYS, "OK");
        } else {
            LOG_ERROR(SYS, "Err");
        }
    } else {
        LOG_ERROR(SYS, "Usage: ping if");
    }
    return res;
}

bool chat_command(int32_t argc, char* argv[]) {
    bool res = false;
    uint8_t tx_array[256] = {0};
    memset(tx_array, 0x00, sizeof(tx_array));
    uint32_t tx_array_len = 0;
    uint8_t interface = IF_NONE;
    uint8_t lifetime = 0;
    uint8_t ack = 5;
    if(1 <= argc) {
        res = try_str2array(argv[0], tx_array, sizeof(tx_array), &tx_array_len);
        if(false == res) {
            strncpy((char*)tx_array, argv[0], sizeof(tx_array));
            tx_array_len = (uint16_t)strlen((char*)tx_array) + 1U;
            res = true;
        }
    }

    if(2 <= argc) {
        res = try_str2uint8(argv[1], &lifetime);
    }

    if(3 <= argc) {
        res = try_str2uint8(argv[2], &interface);
    }

    if(4 <= argc) {
        res = try_str2uint8(argv[3], &ack);
    }

    if( (0 == argc) || (4 < argc) ) {
        LOG_ERROR(SYS, "Usage: char text ttl if");
        res = false;
    }

    if(res) {
        res = tbfp_send_chat(tx_array, tx_array_len, (Interfaces_t)interface,
                             lifetime,
                             (TbfpAck_t) ack);
        if(res) {
            LOG_INFO(SYS, "ok [%s]", tx_array);
            res = print_mem(tx_array, tx_array_len, false, true, true, false);
        } else {
            LOG_ERROR(SYS, "chat error");
        }
    }
    return res;
}
