#include "telematic_commands.h"

#include <inttypes.h>
#include <stdio.h>
#include <time.h>

#include "convert.h"
#include "core_driver.h"
#include "debug_info.h"
#include "io_utils.h"
#include "log.h"
#include "lora_drv.h"
#include "tbfp_protocol.h"
#ifdef HAS_ZED_F9P
#include "zed_f9p_drv.h"
#endif

bool ping_command(int32_t argc, char* argv[]){
    bool res = false;
    if(0 == argc) {
        res = true;
        res = tbfp_send_ping(FRAME_ID_PING);
        if(res){
            LOG_INFO(SYS, "OK");
        }else{
            LOG_ERROR(SYS, "Err");
        }
    } else {
        LOG_ERROR(SYS, "Usage: ping");
    }
    return res;
}

bool chat_command(int32_t argc, char* argv[]){
    bool res = false;
    uint8_t tx_array[256] = {0};
    memset(tx_array, 0x00, sizeof(tx_array));
    uint32_t tx_array_len = 0;
    if(1 <= argc) {
        res = try_str2array(argv[0], tx_array, sizeof(tx_array), &tx_array_len);
        if(false == res) {
            strncpy((char*)tx_array, argv[0], sizeof(tx_array));
            tx_array_len = (uint16_t)strlen((char*)tx_array) + 1U;
            res = true;
        }
    }

    if((1 < argc) || (0 == argc)) {
        LOG_ERROR(SYS, "Usage: char text");
        res = false;
    }
    if(true == res) {
        res = tbfp_send_chat(tx_array, tx_array_len);
        if(res) {
            LOG_INFO(SYS, "ok [%s]",tx_array);
            res = print_mem(tx_array,tx_array_len,false,true,true);
        } else {
            LOG_ERROR(SYS, "chat error");
        }
    }
    return res;
}


