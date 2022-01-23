#include "led_commands.h"

#include <inttypes.h>
#include <stdio.h>

#include "convert.h"
#include "led_drv.h"
#include "data_utils.h"
#ifdef HAS_LOG
#include "io_utils.h"
#include "log.h"
#include "writer_config.h"
#include "table_utils.h"
#endif

bool led_get_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(0 == argc) {
        res = true;
        static const table_col_t cols[] = {
        		{8, "period"},
    			{5, "duty"},
    			{5, "phase"},           };
        table_header(&(curWriterPtr->s), cols, ARRAY_SIZE(cols));
        uint8_t i=0;
        for(i=0;i<ARRAY_SIZE(Led);i++){
	      io_printf(TSEP);
          io_printf(" %u  " TSEP, Led[i].period_ms);
          io_printf(" %u  " TSEP, Led[i].duty);
          io_printf(" %u  " TSEP, Led[i].phase_ms);
          io_printf(" %u  " TSEP, Led[i].mode);
          io_printf(CRLF);
        }
        table_row_bottom(&(curWriterPtr->s), cols, ARRAY_SIZE(cols));
    } else {
        LOG_ERROR(SYS, "Usage: lg ");
    }
    return res;
}

bool led_set_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(4 == argc) {
        res = true;
        uint8_t led_num = 0;
        if(true == res) {
            res = try_str2uint8(argv[0], &led_num);
            if(false == res) {
                LOG_ERROR(SYS, "Unable to extract led_num %s", argv[0]);
            }
        }


        if(true == res) {
            res = try_str2uint32(argv[0], &Led[led_num].period_ms);
            if(false == res) {
                LOG_ERROR(SYS, "Unable to extract period %s", argv[0]);
            }
        }

        if(true == res) {
            res = try_str2uint8(argv[1], &Led[led_num].duty);
            if(false == res) {
                LOG_ERROR(SYS, "Unable to extract duty %s", argv[1]);
            }
        }

        if(true == res) {
            res = try_str2uint32(argv[2], &Led[led_num].phase_ms);
            if(false == res) {
                LOG_ERROR(SYS, "Unable to extract phase %s", argv[2]);
            }
        }

    } else {
        LOG_ERROR(SYS, "Usage: ls num period_ms duty phase_ms");
        LOG_INFO(SYS, "period");
        LOG_INFO(SYS, "duty 0....100 ");
        LOG_INFO(SYS, "phase");
    }
    return res;
}
