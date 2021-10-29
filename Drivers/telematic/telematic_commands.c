#include "telematic_commands.h"

#include <inttypes.h>
#include <stdio.h>
#include <time.h>

#include "convert.h"
#include "core_driver.h"
#include "io_utils.h"
#include "log.h"
#include "lora_drv.h"
#include "tbfp_protocol.h"
#include "zed_f9p_drv.h"

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



