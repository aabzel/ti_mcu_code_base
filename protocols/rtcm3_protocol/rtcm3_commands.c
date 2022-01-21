
#include "rtcm3_commands.h"

#include <stdbool.h>
#include <stdio.h>

#include "convert.h"
#include "data_utils.h"
#include "io_utils.h"
#include "log.h"
#include "system.h"
#include "rtcm3_protocol.h"
#include "table_utils.h"
#include "writer_generic.h"
#include "writer_config.h"

static bool rtcm3_diag(void) {
    bool res = false;
    uint8_t interface = 0;
    static const table_col_t cols[] = {{7, "if"},
                                       {5, "loF"},
                                       {9, "lostLoRa"},
                                       {8, "lost,%"},
                                       {9, "lostUart"},
                                       {9, "rxCnt"},
                                       {9, "crcErCnt"},
#ifdef HAS_DEBUG
                                       {9, "DiffRxCnt"},
                                       {9, "preCnt"},
                                       {9, "ErCnt"},
                                       {9, "minLen"},
                                       {9, "maxLen"},
#endif /*HAS_DEBUG*/
    };
    table_header(&(curWriterPtr->s), cols, ARRAY_SIZE(cols));
    for(interface = 0; interface < ARRAY_SIZE(Rtcm3Protocol); interface++) {
        io_printf(TSEP);
        io_printf(" %5s " TSEP, interface2str((Interfaces_t)interface));
        io_printf("  %1u  " TSEP, Rtcm3Protocol[interface].forwarding[IF_LORA]);
        io_printf(" %7u " TSEP,  Rtcm3Protocol[interface].lost_pkt_cnt[IF_LORA]);
        double lora_lost_pkt_pc = ((double)100*Rtcm3Protocol[interface].lost_pkt_cnt[IF_LORA])/((double)Rtcm3Protocol[interface].rx_pkt_cnt);
        io_printf("   %3.1f  " TSEP,(double) lora_lost_pkt_pc);
        io_printf(" %7u " TSEP, Rtcm3Protocol[interface].uart_lost_pkt_cnt);
        io_printf(" %7u " TSEP, Rtcm3Protocol[interface].rx_pkt_cnt);
        io_printf(" %7u " TSEP, Rtcm3Protocol[interface].crc_err_cnt);
#ifdef HAS_DEBUG
        Rtcm3Protocol[interface].diff_rx_pkt_cnt = Rtcm3Protocol[interface].rx_pkt_cnt-Rtcm3Protocol[interface].prev_rx_pkt_cnt;
        io_printf(" %7u " TSEP, Rtcm3Protocol[interface].diff_rx_pkt_cnt);
        Rtcm3Protocol[interface].prev_rx_pkt_cnt = Rtcm3Protocol[interface].rx_pkt_cnt;
        io_printf(" %7u " TSEP, Rtcm3Protocol[interface].preamble_cnt);
        io_printf(" %7u " TSEP, Rtcm3Protocol[interface].err_cnt);
        io_printf(" %7u " TSEP, Rtcm3Protocol[interface].min_len);
        io_printf(" %7u " TSEP, Rtcm3Protocol[interface].max_len);
#endif /*HAS_DEBUG*/
        io_printf(CRLF);
        res = true;
    }
    table_row_bottom(&(curWriterPtr->s), cols, ARRAY_SIZE(cols));
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
        res = rtcm3_reset_rx(&Rtcm3Protocol[interface]);
    }
    return res;
}


bool rtcm3_fwd_command(int32_t argc, char* argv[]){
    bool res = false;
    bool status= 0;
    uint8_t interface1 = 0;
    uint8_t interface2 = 0;
    if(2 == argc) {
        res = try_str2uint8(argv[0], &interface1);
        if(false == res) {
            LOG_ERROR(SYS, "Unable to extract interface1 %s", argv[0]);
        }

        res = try_str2uint8(argv[1], &interface2);
        if(false == res) {
            LOG_ERROR(SYS, "Unable to extract interface2 %s", argv[1]);
        }

        res = try_str2bool(argv[2], &status);
        if(false == res) {
            LOG_ERROR(SYS, "Unable to extract status %s", argv[2]);
        }
    } else {
        LOG_ERROR(SYS, "Usage: rtcmf interface1 interface2 status");
    }
    if(res) {
        Rtcm3Protocol[interface1].forwarding[interface2] = status;
    }
    return res;
}
