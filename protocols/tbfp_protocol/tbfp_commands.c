
#include "tbfp_commands.h"

#include <stdbool.h>
#include <stdio.h>

#include "convert.h"
#include "data_utils.h"
#include "io_utils.h"
#include "log.h"
#include "tbfp_protocol.h"
#include "table_utils.h"
#include "writer_generic.h"
#include "writer_config.h"

static bool tbfp_diag(void) {
    bool res = false;
    Interfaces_t interface;
    static const table_col_t cols[] = {
                                       {3, "If"},
                                       {9, "rxCnt"},
                                       {9, "crcErCnt"},
#ifdef HAS_DEBUG
                                       {9, "maxFlow"},
                                       {9, "preCnt"},
                                       {9, "ErCnt"},
                                       {9, "minLen"},
                                       {9, "maxLen"},
#endif /*HAS_DEBUG*/
    };
    table_header(&(curWriterPtr->s), cols, ARRAY_SIZE(cols));
    for(interface=IF_LORA; interface < ARRAY_SIZE(TbfpProtocol); interface++){
        io_printf(TSEP);
        io_printf(" %1u " TSEP, TbfpProtocol[interface].interface);
        io_printf(" %7u " TSEP, TbfpProtocol[interface].rx_pkt_cnt);
        io_printf(" %7u " TSEP, TbfpProtocol[interface].crc_err_cnt);

#ifdef HAS_DEBUG
        io_printf(" %7u " TSEP, TbfpProtocol[interface].max_con_flow);
        io_printf(" %7u " TSEP, TbfpProtocol[interface].preamble_cnt);
        io_printf(" %7u " TSEP, TbfpProtocol[interface].err_cnt);
        io_printf(" %7u " TSEP, TbfpProtocol[interface].min_len);
        io_printf(" %7u " TSEP, TbfpProtocol[interface].max_len);
        io_printf(CRLF);
#endif /*HAS_DEBUG*/
        res = true;
    }

    table_row_bottom(&(curWriterPtr->s), cols, ARRAY_SIZE(cols));
    return res;
}

bool tbfp_diag_command(int32_t argc, char* argv[]) {
    bool res = false;

    if(0 == argc) {
        res = true;
    } else {
        LOG_ERROR(TBFP, "Usage: tbfpd");
    }
    if(res) {
        res = tbfp_diag();
    }
    return res;
}

bool tbfp_send_command(int32_t argc, char* argv[]){
    bool res = false;
    uint8_t array[256] = {0};
    uint32_t array_len = 0;
    uint8_t interface = IF_NONE;
    if(1<=argc){
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


    if(res) {
        res = tbfp_send(array,array_len,(Interfaces_t) interface);
        if(res){
            LOG_INFO(TBFP, "Ok!");
        }else{
            LOG_ERROR(TBFP, "TbfpSendError");
        }

    }else{
        LOG_ERROR(TBFP, "Usage: tbfpds data interface");
    }
    return res;
}



