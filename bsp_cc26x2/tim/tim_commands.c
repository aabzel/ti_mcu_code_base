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

bool tim_diag_ll_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(0 == argc) {
        res = true;
        const table_col_t cols[] = {{5, "GPT"},   {5, "width"},  {5, "tim"}, {14, "Val"},     {8, "PSC"},
                                    {14, "load"}, {14, "match"}, {8, "It"},  {8, "period_ms"}};
        uint8_t part = 0, tim_base_id = 0, width = 0;
        uint32_t val = 0, load = 0, match = 0;
        uint32_t prescaler = 0;
        float calc_period = 0.0f;
        table_header(&dbg_o.s, cols, ARRAY_SIZE(cols));
        for(tim_base_id = 0; tim_base_id < ARRAY_SIZE(TimBaseLut); tim_base_id++) {

            for(part = 0; part < 2; part++) {
                val = TimerValueGet(TimBaseLut[tim_base_id], TimInstLUT[part]);
                prescaler = TimerPrescaleGet(TimBaseLut[tim_base_id], TimInstLUT[part]);
                load = TimerLoadGet(TimBaseLut[tim_base_id], TimInstLUT[part]);
                match = TimerMatchGet(TimBaseLut[tim_base_id], TimInstLUT[part]);
                calc_period = tim_calc_real_period_s(SYS_FREQ, prescaler, load);
                width = tim_get_width(TimBaseLut[tim_base_id]);
                io_printf(TSEP);
                io_printf("  %u  " TSEP, tim_base_id);
                io_printf("  %2u " TSEP, width);
                io_printf("  %s  " TSEP, (0 == part) ? "A" : "B");
                io_printf(" %12u " TSEP, val);
                io_printf(" %6u " TSEP, prescaler);
                io_printf(" %12u " TSEP, load);
                io_printf(" %12u " TSEP, match);
                io_printf(" %6u " TSEP, TimerItem[tim_base_id * 2 + part].tim_it_cnt);
                io_printf(" %6.1f " TSEP, calc_period * 1000.0f);

                io_printf(CRLF);
            }
        }

        table_row_bottom(&dbg_o.s, cols, ARRAY_SIZE(cols));
    } else {
        LOG_ERROR(TIM, "Usage: tdl");
    }

    return res;
}

bool tim_diag_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(0 == argc) {
        res = true;
        const table_col_t cols[] = {{5, "num"}, {14, "Val"}, {14, "rVal"}};
        uint8_t tim_num = 0;
        uint32_t val = 0, fRval = 0;
        table_header(&dbg_o.s, cols, ARRAY_SIZE(cols));
        for(tim_num = 0; tim_num < ARRAY_SIZE(TimerItem); tim_num++) {
            if(TimerItem[tim_num].hTimer) {
                fRval = GPTimerCC26XX_getFreeRunValue(TimerItem[tim_num].hTimer);
                val = GPTimerCC26XX_getValue(TimerItem[tim_num].hTimer);
                io_printf(TSEP);
                io_printf("  %u  " TSEP, tim_num);
                io_printf(" %12u " TSEP, val);
                io_printf(" %12u " TSEP, fRval);
                io_printf(CRLF);
            }
        }
        table_row_bottom(&dbg_o.s, cols, ARRAY_SIZE(cols));
    } else {
        LOG_ERROR(TIM, "Usage: td");
    }

    return res;
}

bool tim_set_prescaler_command(int32_t argc, char* argv[]) {
    bool res = false;
    uint8_t tim_base_id = 0;
    uint8_t tim_inst = 0;
    uint8_t prescaler = 0;
    if(3 == argc) {
        res = true;
        if(true == res) {
            res = try_str2uint8(argv[0], &tim_base_id);
            if(false == res) {
                LOG_ERROR(SYS, "Unable to extract tim_base_id %s", argv[0]);
            }
            if(ARRAY_SIZE(TimBaseLut) <= tim_base_id) {
                res = false;
            }
        }

        if(true == res) {
            res = try_str2uint8(argv[1], &tim_inst);
            if(false == res) {
                LOG_ERROR(SYS, "Unable to extract tim_inst %s", argv[1]);
            }
            if(4 <= tim_inst) {
                res = false;
            }
        }

        if(true == res) {
            res = try_str2uint8(argv[2], &prescaler);
            if(false == res) {
                LOG_ERROR(SYS, "Unable to extract prescaler %s", argv[2]);
            }
        }
    } else {
        LOG_ERROR(TIM, "Usage: tsp base inst prescaler");
    }

    if(res) {
        TimerPrescaleSet(TimBaseLut[tim_base_id], TimInstLUT[tim_inst], prescaler);
        LOG_INFO(TIM, "OK");
    }

    return res;
}

bool tim_set_laod_command(int32_t argc, char* argv[]) {
    bool res = false;
    uint8_t tim_base_id = 0;
    uint8_t tim_inst = 0;
    uint32_t load = 0;
    if(3 == argc) {
        res = true;
        if(true == res) {
            res = try_str2uint8(argv[0], &tim_base_id);
            if(false == res) {
                LOG_ERROR(SYS, "Unable to extract tim_base_id %s", argv[0]);
            }
            if(ARRAY_SIZE(TimBaseLut) <= tim_base_id) {
                res = false;
            }
        }

        if(true == res) {
            res = try_str2uint8(argv[1], &tim_inst);
            if(false == res) {
                LOG_ERROR(SYS, "Unable to extract tim_inst %s", argv[1]);
            }
            if(4 <= tim_inst) {
                res = false;
            }
        }

        if(true == res) {
            res = try_str2uint32(argv[2], &load);
            if(false == res) {
                LOG_ERROR(SYS, "Unable to extract load %s", argv[2]);
            }
        }
    } else {
        LOG_ERROR(TIM, "Usage: tsl base inst load");
    }

    if(res) {
        TimerLoadSet(TimBaseLut[tim_base_id], TimInstLUT[tim_inst], load);
        LOG_INFO(TIM, "OK");
    }

    return res;
}

bool tim_set_period_command(int32_t argc, char* argv[]) {
    bool res = false;
    uint8_t tim_index = 0;
    uint32_t pesiod_ms = 1000;
    if(2 == argc) {
        res = true;
        if(true == res) {
            res = try_str2uint8(argv[0], &tim_index);
            if(false == res) {
                LOG_ERROR(SYS, "Unable to extract index %s", argv[0]);
            }
            if(ARRAY_SIZE(TimerItem) <= tim_index) {
                res = false;
            }
        }

        if(true == res) {
            res = try_str2uint32(argv[1], &pesiod_ms);
            if(false == res) {
                LOG_ERROR(SYS, "Unable to extract pesiod_ms %s", argv[1]);
            }
        }
    } else {
        LOG_ERROR(TIM, "Usage: tspr tim_index pesiod_ms");
        res = false;
    }
    if(res) {
        uint32_t prescaler = 0;
        uint32_t load = 0;
        res = tim_calc_registers(pesiod_ms, SYS_FREQ, prescaler, &load, 0xFFFFFFFF);
        if(res) {
            LOG_INFO(SYS, "prescaler: %u load: %u", prescaler, load);
            TimerPrescaleSet(gptimerCC26xxHWAttrs[tim_index].baseAddr, TimInstLUT[tim_index % 2], prescaler);
            GPTimerCC26XX_setLoadValue(TimerItem[tim_index].hTimer, load);
        }
    }
    return res;
}
