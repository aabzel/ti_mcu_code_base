#include "interrupt_commands.h"

#include <interrupt.h>
#include <inttypes.h>
#include <stdio.h>

#include "convert.h"
#include "data_utils.h"
//#include "debug_info.h"
#include "log.h"
#include "table_utils.h"
#include "writer_config.h"

// Table 5-1. Exception Types
bool it_diag_command(int32_t argc, char* argv[]) {
    bool res = false;
    bool pen = false;
    uint32_t index = 0;
    if(0 == argc) {
        static const table_col_t cols[] = {
            {5, "index"},
            {5, "Prior"},
            {5, "Prior"},
            {5, "pend"},
        };
        table_header(&(curWriterPtr->s), cols, ARRAY_SIZE(cols));
        for(index = 1; index <= 53; index++) {
            int32_t priority = IntPriorityGet(index);
            pen = IntPendGet(index);
            io_printf(TSEP);
            io_printf(" %3u " TSEP, index);
            io_printf(" %3u " TSEP, priority);
            io_printf(" 0x%02x " TSEP, priority);
            io_printf(" %u " TSEP, pen);
            io_printf(CRLF);
        }
        table_row_bottom(&(curWriterPtr->s), cols, ARRAY_SIZE(cols));
        res = true;
    }
    if(1 == argc) {
        res = try_str2uint32(argv[0], &index);
        if(false == res) {
            LOG_ERROR(INT, "Unable to parse index");
        }
        int32_t ret = IntPriorityGet(index);
        res = IntPendGet(index);
        LOG_INFO(INT, "index:%u priority:%d=0x%x %s pending", index, ret, ret, (true == res) ? "" : "not");
        res = true;
    } else if(false == res) {
        LOG_ERROR(INT, "Usage: itd indexVectorTable");
    }

    return res;
}
