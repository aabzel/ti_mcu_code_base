
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

static bool tbfp_diag(void) {
    bool res = false;

    static const table_col_t cols[] = {
                                       {9, "rxCnt"},
                                       {9, "preCnt"},
                                       {9, "crcErCnt"},
                                       {9, "ErCnt"},
                                       {9, "minLen"},
                                       {9, "maxLen"}};
    table_header(&dbg_o.s, cols, ARRAY_SIZE(cols));

        io_printf(TSEP);
        io_printf(" %7u " TSEP, TbfpPorotocol.rx_pkt_cnt);
#ifdef HAS_DEBUG
        io_printf(" %7u " TSEP, TbfpPorotocol.preamble_cnt);
#endif /*HAS_DEBUG*/
        io_printf(" %7u " TSEP, TbfpPorotocol.crc_err_cnt);

#ifdef HAS_DEBUG
        io_printf(" %7u " TSEP, TbfpPorotocol.err_cnt);
        io_printf(" %7u " TSEP, TbfpPorotocol.min_len);
        io_printf(" %7u " TSEP, TbfpPorotocol.max_len);
        io_printf(CRLF);
#endif /*HAS_DEBUG*/
        res = true;

    table_row_bottom(&dbg_o.s, cols, ARRAY_SIZE(cols));
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



