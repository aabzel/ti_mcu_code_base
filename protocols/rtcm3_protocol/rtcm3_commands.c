
#include "rtcm3_commands.h"

#include <stdbool.h>
#include <stdio.h>

#include "convert.h"
#include "data_utils.h"
#include "io_utils.h"
#include "log.h"
#include "rtcm3_protocol.h"
#include "table_utils.h"
#include "writer_generic.h"

static const char* interfacefRtcmLuTable[RTCM_IF_CNT] = {"UART", "LoRa"};

static bool rtcm3_diag(void) {
    bool res = false;
    uint8_t interface = 0;
    static const table_col_t cols[] = {{6, "if"},{5, "loF"},
                                       {9, "lostLoRa"},
                                       {9, "lostUart"},
                                       {9, "rxCnt"},
                                       {9, "preCnt"},
                                       {9, "crcErCnt"},
                                       {9, "ErCnt"},
                                       {9, "minLen"},
                                       {9, "maxLen"}};
    table_header(&dbg_o.s, cols, ARRAY_SIZE(cols));
    for(interface = 0; interface < RTCM_IF_CNT; interface++) {
        io_printf(TSEP);
        io_printf(" %4s " TSEP, interfacefRtcmLuTable[interface]);
        io_printf("  %1u  " TSEP, Rtcm3Porotocol[interface].lora_fwd);
        io_printf(" %7u " TSEP,  Rtcm3Porotocol[interface].lora_lost_pkt_cnt);
        io_printf(" %7u " TSEP,  Rtcm3Porotocol[interface].uart_lost_pkt_cnt);
        io_printf(" %7u " TSEP, Rtcm3Porotocol[interface].rx_pkt_cnt);
#ifdef HAS_DEBUG
        io_printf(" %7u " TSEP, Rtcm3Porotocol[interface].preamble_cnt);
#endif /*HAS_DEBUG*/
        io_printf(" %7u " TSEP, Rtcm3Porotocol[interface].crc_err_cnt);

#ifdef HAS_DEBUG
        io_printf(" %7u " TSEP, Rtcm3Porotocol[interface].err_cnt);
        io_printf(" %7u " TSEP, Rtcm3Porotocol[interface].min_len);
        io_printf(" %7u " TSEP, Rtcm3Porotocol[interface].max_len);
        io_printf(CRLF);
#endif /*HAS_DEBUG*/
        res = true;
    }
    table_row_bottom(&dbg_o.s, cols, ARRAY_SIZE(cols));
    return res;
}

bool rtcm3_diag_command(int32_t argc, char* argv[]) {
    bool res = false;

    if(0 == argc) {
        res = true;
    } else {
        LOG_ERROR(SYS, "Usage: rtcmd");
    }

    if(res) {
        res = rtcm3_diag();
    }
    return res;
}

bool rtcm3_reset_command(int32_t argc, char* argv[]) {
    bool res = false;
    uint8_t interface = 0;
    if(1 == argc) {
        res = try_str2uint8(argv[0], &interface);
        if(false == res) {
            LOG_ERROR(SYS, "Unable to extract interface %s", argv[0]);
        }
    } else {
        LOG_ERROR(SYS, "Usage: rtcmr interface");
    }
    if(res) {
        res = rtcm3_reset_rx(&Rtcm3Porotocol[interface]);
    }
    return res;
}


bool rtcm3_fwd_command(int32_t argc, char* argv[]){
    bool res = false;
    bool status= 0;
    uint8_t interface = 0;
    if(2 == argc) {
        res = try_str2uint8(argv[0], &interface);
        if(false == res) {
            LOG_ERROR(SYS, "Unable to extract interface %s", argv[0]);
        }

        res = try_str2bool(argv[1], &status);
        if(false == res) {
            LOG_ERROR(SYS, "Unable to extract status %s", argv[1]);
        }
    } else {
        LOG_ERROR(SYS, "Usage: rtcmf interface status");
    }
    if(res) {
        Rtcm3Porotocol[interface].lora_fwd = status;
    }
    return res;
}
