
#include "tbfp_commands.h"

#include <stdbool.h>
#include <stdio.h>

#include "convert.h"
#include "data_utils.h"
#include "io_utils.h"
#include "log.h"
#include "none_blocking_pause.h"
#include "system.h"
#include "table_utils.h"
#include "tbfp_protocol.h"
#include "tbfp_retx_diag.h"
#include "writer_config.h"
#include "writer_generic.h"


static bool tbfp_diag_retx(void) {
    bool res = false;
    Interfaces_t interface;
    static const table_col_t cols[] = {
        {8, "interf"},
        {9, "spinCnt"},
        {9, "state"},
        {9, "LackTxDone"},
        {9, "AckRx"},
        {9, "AckTx"},
        {9, "ReTx"},
        {9, "NoRepl"},
        {7, "Err"},
    };
    table_header(&(curWriterPtr->s), cols, ARRAY_SIZE(cols));
    for(interface = (Interfaces_t)0; interface < ARRAY_SIZE(TbfpProtocol); interface++) {
        if(TbfpProtocol[interface].interface==interface){
            io_printf(TSEP);
            io_printf(" %6s " TSEP, interface2str(TbfpProtocol[interface].interface));
            io_printf(" %7u " TSEP, TbfpProtocol[interface].ReTxFsm.spin_cnt);
            io_printf(" %7s " TSEP, tbfp_retx_state2str(TbfpProtocol[interface].ReTxFsm.state));
            io_printf(" %7u " TSEP, TbfpProtocol[interface].ReTxFsm.err_tx_done);
            io_printf(" %7u " TSEP, TbfpProtocol[interface].ReTxFsm.ack_rx_cnt);
            io_printf(" %7u " TSEP, TbfpProtocol[interface].ReTxFsm.ack_tx_cnt);
            io_printf(" %7u " TSEP, TbfpProtocol[interface].ReTxFsm.retx_cnt);
            io_printf(" %7u " TSEP, TbfpProtocol[interface].ReTxFsm.no_reply_cnt);
            io_printf(" %5u " TSEP, TbfpProtocol[interface].ReTxFsm.err_cnt);
            io_printf(CRLF);
            res = true;
        }
    }

    table_row_bottom(&(curWriterPtr->s), cols, ARRAY_SIZE(cols));
    return res;
}

static bool tbfp_diag(void) {
    bool res = false;
    Interfaces_t interface;
    static const table_col_t cols[] = {
        {8, "interf"},
        {9, "rxCnt"},
        {9, "txCnt"},

#ifdef HAS_DEBUG
#ifdef HAS_TBFP_FLOW_CONTROL
        {9, "maxFlow"},
        {9, "curFlow"},
#endif
        {9, "preCnt"},
        {9, "minLen"},
        {9, "maxLen"},
#endif /*HAS_DEBUG*/
    };
    table_header(&(curWriterPtr->s), cols, ARRAY_SIZE(cols));
    for(interface = (Interfaces_t)0; interface < ARRAY_SIZE(TbfpProtocol); interface++) {
        if(TbfpProtocol[interface].interface==interface){

            io_printf(TSEP);

            io_printf(" %6s " TSEP, interface2str(TbfpProtocol[interface].interface));
            io_printf(" %7u " TSEP, TbfpProtocol[interface].rx_pkt_cnt);
            io_printf(" %7u " TSEP, TbfpProtocol[interface].tx_pkt_cnt);


    #ifdef HAS_DEBUG
    #ifdef HAS_TBFP_FLOW_CONTROL
            io_printf(" %7u " TSEP, TbfpProtocol[interface].max_con_flow);
            io_printf(" %7u " TSEP, TbfpProtocol[interface].con_flow);
    #endif
            io_printf(" %7u " TSEP, TbfpProtocol[interface].preamble_cnt);
            io_printf(" %7u " TSEP, TbfpProtocol[interface].min_len);
            io_printf(" %7u " TSEP, TbfpProtocol[interface].max_len);
            io_printf(CRLF);
    #endif /*HAS_DEBUG*/
            res = true;
        }
    }

    table_row_bottom(&(curWriterPtr->s), cols, ARRAY_SIZE(cols));
    return res;
}

bool tbfp_rate_diag(void) {
    bool res = false;
    Interfaces_t interface;
    static const table_col_t cols[] = {
        {8, "interf"},
        {9, "rxMin"},
        {9, "rxCur"},
        {9, "rxMax"},

        {9, "txMin"},
        {9, "txCur"},
        {9, "txMax"}
    };
    table_header(&(curWriterPtr->s), cols, ARRAY_SIZE(cols));
    for(interface = (Interfaces_t)0; interface < ARRAY_SIZE(TbfpProtocol); interface++) {
        if(TbfpProtocol[interface].interface==interface){
            io_printf(TSEP);
            io_printf(" %6s " TSEP, interface2str(TbfpProtocol[interface].interface));
            io_printf(" %7u " TSEP, TbfpProtocol[interface].rx_rate.min);
            io_printf(" %7u " TSEP, TbfpProtocol[interface].rx_rate.cur);
            io_printf(" %7u " TSEP, TbfpProtocol[interface].rx_rate.max);
            io_printf(" %7u " TSEP, TbfpProtocol[interface].tx_rate.min);
            io_printf(" %7u " TSEP, TbfpProtocol[interface].tx_rate.cur);
            io_printf(" %7u " TSEP, TbfpProtocol[interface].tx_rate.max);
            io_printf(CRLF);
            res = true;
        }
    }

    table_row_bottom(&(curWriterPtr->s), cols, ARRAY_SIZE(cols));
    return res;
}


static bool tbfp_error(void) {
    bool res = false;
    Interfaces_t interface;
    static const table_col_t cols[] = {
        {8, "interf"},
        {9, "LackFrame"},
        {9, "LenErCnt"},
        {9, "crcErCnt"},
#ifdef HAS_DEBUG
#ifdef HAS_TBFP_FLOW_CONTROL
        {9, "TornFlow"},
        {9, "LostRx"},
#endif
        {9, "TxErCnt"},
         {9, "ErCnt"},
#endif /*HAS_DEBUG*/
    };
    table_header(&(curWriterPtr->s), cols, ARRAY_SIZE(cols));
    for(interface = (Interfaces_t)0; interface < ARRAY_SIZE(TbfpProtocol); interface++) {
        if(TbfpProtocol[interface].interface==interface){

            io_printf(TSEP);

            io_printf(" %6s " TSEP, interface2str(TbfpProtocol[interface].interface));
            io_printf(" %7u " TSEP, TbfpProtocol[interface].lack_frame_in_data);
            io_printf(" %7u " TSEP, TbfpProtocol[interface].len_err_cnt);
            io_printf(" %7u " TSEP, TbfpProtocol[interface].crc_err_cnt);
    #ifdef HAS_DEBUG
    #ifdef HAS_TBFP_FLOW_CONTROL
            io_printf(" %7u " TSEP, TbfpProtocol[interface].flow_torn_cnt);
            io_printf(" %7u " TSEP, TbfpProtocol[interface].lost_rx_frames);
    #endif
            io_printf(" %7u " TSEP, TbfpProtocol[interface].err_tx_cnt);
            io_printf(" %7u " TSEP, TbfpProtocol[interface].err_cnt);
            io_printf(CRLF);
    #endif /*HAS_DEBUG*/
            res = true;
        }
    }

    table_row_bottom(&(curWriterPtr->s), cols, ARRAY_SIZE(cols));
    return res;
}

bool tbfp_diag_retx_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(0 <= argc) {
        res = true;
    }
    if(res) {
        res = tbfp_diag_retx();
    }
    return res;
}



bool tbfp_diag_command(int32_t argc, char* argv[]) {
    bool res = false;
    uint8_t interface = IF_NONE;

    if(0 <= argc) {
        res = true;
    }

    if(1 <= argc) {
        res = try_str2uint8(argv[0], &interface);
        if(false == res) {
            LOG_ERROR(SYS, "Unable to extract if %s", argv[0]);
        }
    }

#ifdef HAS_DEBUG
    if(2 <= argc) {
        res = try_str2bool(argv[1], &TbfpProtocol[interface].debug);
        if(false == res) {
            LOG_ERROR(SYS, "Unable to extract state %s", argv[1]);
        }
    }
#endif
    if(2 < argc) {
        LOG_ERROR(TBFP, "Usage: tbfpd if debug");
    }
    if(res) {
        res = tbfp_diag();
    }
    return res;
}

bool tbfp_error_command(int32_t argc, char* argv[]){
    bool res = false;
    if(0 == argc) {
        res = true;
    }
    if(res){
        res = tbfp_error();
    }else{
        LOG_ERROR(TBFP, "Usage: tbfpd if debug");
    }

    return res;
}

bool tbfp_rate_command(int32_t argc, char* argv[]){
    bool res = false;
    if(0 == argc) {
        res = true;
    }
    if(res){
        res = tbfp_rate_diag();
    }else{
        LOG_ERROR(TBFP, "Usage: tbt");
    }
    return res;
}

bool tbfp_send_command(int32_t argc, char* argv[]) {
    bool res = false;
    uint8_t array[256] = {0};
    uint32_t array_len = 0;
    uint8_t livetime = 0;
    uint8_t ack = 0;
    uint8_t interface = IF_NONE;
    if(1 <= argc) {
        res = try_str2array(argv[0], array, sizeof(array), &array_len);
        if(false == res) {
            LOG_ERROR(TBFP, "Unable to extract data %s", argv[0]);
        }
    }
    if(2 <= argc) {
        res = try_str2uint8(argv[1], &interface);
        if(false == res) {
            LOG_ERROR(TBFP, "UnableToParseInterface");
        }
    }
    if(3 <= argc) {
        res = try_str2uint8(argv[2], &livetime);
        if(false == res) {
            LOG_ERROR(TBFP, "UnableToParseLivetime");
        }
    }
    if(4 <= argc) {
        res = try_str2uint8(argv[3], &ack);
        if(false == res) {
            LOG_ERROR(TBFP, "UnableToParseAck");
        }
    }

    if(res) {
        res = tbfp_send(array, array_len,
                        (Interfaces_t)interface,
                        livetime,
                        (TbfpAck_t)ack);
        if(res) {
            LOG_INFO(TBFP, "Ok!");
        } else {
            LOG_ERROR(TBFP, "TbfpSendError");
        }
    } else {
        LOG_ERROR(TBFP, "Usage: tbfpds data interface");
    }
    return res;
}

bool tbfp_send_hi_load_command(int32_t argc, char* argv[]) {
    bool res = false;
    uint32_t attempt = 0;
    uint32_t len = 0;
    uint32_t pause_ms = 0;
    uint8_t interface = IF_NONE;
    uint8_t array[255 - TBFP_OVERHEAD_SIZE] = {0};
    memset(array, 0, len);

    if(1 <= argc) {
        res = try_str2uint32(argv[0], &len);
        if(false == res) {
            LOG_ERROR(TBFP, "UnableToParseLen");
        }
    }
    if(2 <= argc) {
        res = try_str2uint32(argv[1], &attempt);
        if(false == res) {
            LOG_ERROR(TBFP, "UnableToParseAtt");
        }
    }

    if(3 <= argc) {
        res = try_str2uint32(argv[2], &pause_ms);
        if(false == res) {
            LOG_ERROR(TBFP, "UnableToParsePause");
        }
    }

    if(4 <= argc) {
        res = try_str2uint8(argv[3], &interface);
        if(false == res) {
            LOG_ERROR(TBFP, "UnableToParseInterface");
        }
    }

    if(4 != argc) {
        LOG_ERROR(TBFP, "Usage: tbfsh"
                        " len"
                        " attempt"
                        " pause_ms"
                        " interf");
        return res;
    }

    if(res) {
        if(len <= sizeof(array)) {
            uint32_t i = 0;
            for(i = 0; i < attempt; i++) {
                memset(array, ((uint8_t)i), len);
                res = tbfp_send(array, len, (Interfaces_t)interface, 0, ACK_NO_NEED);
                wait_in_loop_ms(pause_ms);
            }
        }
    }
    return res;
}
