#include "zed_f9p_diag.h"
 
#include <stdint.h>

#include "zed_f9p_drv.h"

const char *rtk_mode2str(uint8_t rtk_code){
    const char *name ="error";
    switch(rtk_code){
    case RTK_BASE:
        name ="base";
        break;
    case RTK_ROVER:
        name ="rover";
        break;
    default:
        name ="undef";
        break;
    }
    return name;
}

const char *rtk_channel2str(uint8_t code){
    const char *name ="error";
    switch(code){
    case RTK_CH_LORA:
        name ="LoRa";
        break;
    case RTK_CH_RS232:
        name ="RS232";
        break;
    default:
        name ="Error";
        break;
    }
    return name;
}

