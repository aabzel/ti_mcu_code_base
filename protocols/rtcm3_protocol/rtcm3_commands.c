
#include "rtcm3_commands.h"

#include <stdbool.h>
#include <stdio.h>

#include "convert.h"
#include "data_utils.h"
#include "io_utils.h"
#include "log.h"
#include "table_utils.h"
#include "rtcm3_protocol.h"
#include "writer_generic.h"

static const char *interfacefRtcmLuTable[RTCM_IF_CNT]={"UART","LoRa"};

static bool rtcm3_diag(uint8_t interface) {
    bool res = false;
    if(interface<RTCM_IF_CNT) {
       io_printf("interface: %s" CRLF, interfacefRtcmLuTable[interface]);
       io_printf("rx pkt cnt: %u" CRLF, Rtcm3Porotocol[interface].rx_pkt_cnt);
       io_printf("crc err cnt: %u" CRLF, Rtcm3Porotocol[interface].crc_err_cnt);

#ifdef HAS_DEBUG
       io_printf("min len: %u" CRLF, Rtcm3Porotocol[interface].min_len);
       io_printf("max len: %u" CRLF, Rtcm3Porotocol[interface].max_len);
#endif /*HAS_DEBUG*/
       res = true;
    }
    return res;
}



bool rtcm3_diag_command(int32_t argc, char* argv[]){
    bool res = false;
    uint8_t interface=0;
    if(0 == argc){
        interface=RT_BOTH_ID;
        res=true;
    }
    if(1 == argc) {
        res = try_str2uint8(argv[0], &interface);
        if(false == res) {
            LOG_ERROR(SYS, "Unable to extract interface %s", argv[0]);
        }
    } else {
        LOG_ERROR(SYS, "Usage: rtcmd if");
    }

    if(res){
        if (RT_BOTH_ID == interface) {
            res = rtcm3_diag(RT_UART_ID);
            res = rtcm3_diag(RT_LORA_ID);
        }else{
           res = rtcm3_diag(interface);
        }
    }
    return res;
}

bool rtcm3_reset_command(int32_t argc, char* argv[]){
    bool res = false;
    uint8_t interface=0;
    if(1 == argc) {
        res = try_str2uint8(argv[0], &interface);
        if(false == res) {
            LOG_ERROR(SYS, "Unable to extract interface %s", argv[0]);
        }
    } else {
        LOG_ERROR(SYS, "Usage: rtcmr interface");
    }
    if(res){
        res = rtcm3_reset_rx(&Rtcm3Porotocol[interface] );
    }
    return res;
}
