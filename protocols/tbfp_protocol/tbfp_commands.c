
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
        LOG_ERROR(SYS, "Usage: tbfpd");
    }
    if(res) {
        res = tbfp_diag();
    }
    return res;
}



