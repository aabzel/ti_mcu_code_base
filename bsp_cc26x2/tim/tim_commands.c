#include "tim_commands.h"

#include <hw_memmap.h>
#include <inttypes.h>
#include <timer.h>

#include "clocks.h"
#include "convert.h"
#include "data_utils.h"
#include "log.h"
#include "table_utils.h"
#include "tim_drv.h"

bool tim_diag_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(0 == argc) {
        res = true;
        const table_col_t cols[] = {{5, "GPT"},  {5, "tim"},   {8, "Val"}, {8, "PSC"},
                                    {8, "load"}, {8, "match"}, {8, "It"},  {8, "period_ms"}};
        uint8_t i = 0, tim_base_id = 0;
        uint32_t val, load, match;
        uint32_t prescaler;
        float calc_period = 0.0f;
        table_header(&dbg_o.s, cols, ARRAY_SIZE(cols));
        for(tim_base_id = 0; tim_base_id < ARRAY_SIZE(TimBaseLut); tim_base_id++) {

            for(i = 0; i < ARRAY_SIZE(TimInstLUT); i++) {
                val = TimerValueGet(TimBaseLut[tim_base_id], TimInstLUT[i]);
                prescaler = TimerPrescaleGet(TimBaseLut[tim_base_id], TimInstLUT[i]);
                load = TimerLoadGet(TimBaseLut[tim_base_id], TimInstLUT[i]);
                match = TimerMatchGet(TimBaseLut[tim_base_id], TimInstLUT[i]);
                calc_period = tim_calc_real_period_s(SYS_FREQ, prescaler, load);
                io_printf(TSEP);
                io_printf("  %u  " TSEP, tim_base_id);
                io_printf("  %s  " TSEP, (0 == i) ? "A" : "B");
                io_printf(" %6u " TSEP, val);
                io_printf(" %6u " TSEP, prescaler);
                io_printf("  %u " TSEP, load);
                io_printf(" %6u " TSEP, match);
                io_printf(" %6u " TSEP, TimerItem[tim_base_id*2+i].tim_it_cnt);
                io_printf(" %6.1f " TSEP, calc_period * 1000.0f);

                io_printf(CRLF);
            }
        }

        table_row_bottom(&dbg_o.s, cols, ARRAY_SIZE(cols));
    } else {
        LOG_ERROR(TIM, "Usage: td");
    }

    return res;
}
