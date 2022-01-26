
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
#include "writer_config.h"
#include "writer_generic.h"

static bool tbfp_diag(void) {
    bool res = false;
    Interfaces_t interface;
    static const table_col_t cols[] = {
        {8, "interf"},  {9, "rxCnt"},   {9, "txCnt"},  {9, "crcErCnt"},
#ifdef HAS_DEBUG
#ifdef HAS_TBFP_FLOW_CONTROL
        {9, "maxFlow"}, {9, "curFlow"},
#endif
        {9, "preCnt"},  {9, "ErCnt"},   {9, "minLen"}, {9, "maxLen"},   {7, "debug"},
#endif /*HAS_DEBUG*/
    };
    table_header(&(curWriterPtr->s), cols, ARRAY_SIZE(cols));
    for(interface = IF_LORA; interface < ARRAY_SIZE(TbfpProtocol); interface++) {
        io_printf(TSEP);
        io_printf(" %6s " TSEP, interface2str(TbfpProtocol[interface].interface));
        io_printf(" %7u " TSEP, TbfpProtocol[interface].rx_pkt_cnt);
        io_printf(" %7u " TSEP, TbfpProtocol[interface].tx_pkt_cnt);
        io_printf(" %7u " TSEP, TbfpProtocol[interface].crc_err_cnt);

#ifdef HAS_DEBUG
#ifdef HAS_TBFP_FLOW_CONTROL
        io_printf(" %7u " TSEP, TbfpProtocol[interface].max_con_flow);
        io_printf(" %7u " TSEP, TbfpProtocol[interface].con_flow);
#endif
        io_printf(" %7u " TSEP, TbfpProtocol[interface].preamble_cnt);
        io_printf(" %7u " TSEP, TbfpProtocol[interface].err_cnt);
        io_printf(" %7u " TSEP, TbfpProtocol[interface].min_len);
        io_printf(" %7u " TSEP, TbfpProtocol[interface].max_len);
        io_printf(" %5u " TSEP, TbfpProtocol[interface].debug);
        io_printf(CRLF);
#endif /*HAS_DEBUG*/
        res = true;
    }

    table_row_bottom(&(curWriterPtr->s), cols, ARRAY_SIZE(cols));
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

bool tbfp_send_command(int32_t argc, char* argv[]) {
    bool res = false;
    uint8_t array[256] = {0};
    uint32_t array_len = 0;
    uint8_t livetime = 0;
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

    if(res) {
        res = tbfp_send(array, array_len, (Interfaces_t)interface, livetime);
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
                res = tbfp_send(array, len, (Interfaces_t)interface, 0);
                wait_in_loop_ms(pause_ms);
            }
        }
    }
    return res;
}
