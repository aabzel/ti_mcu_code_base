#include "tim_drv.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>


#include "bit_utils.h"
#include "clocks.h"
#include "float_utils.h"
#include "log.h"



float tim_calc_real_period_s(uint32_t cpu_clock, uint32_t prescaler, uint32_t laod) {
    float calc_period_s = 0.0f;
    float cpu_period = 1.0f / ((float)cpu_clock);
    calc_period_s = cpu_period * ((float)((prescaler + 1U) * ((float)laod)));
    return calc_period_s;
}

bool tim_calc_registers(uint32_t period_ms, uint32_t cpu_clock, uint32_t prescaler, uint32_t* out_load,
                        uint32_t max_val) {
    bool res = true;
    uint64_t load = 0;
    float cpu_period = 1.0f / ((float)cpu_clock);
    float calc_period = 0.0f;
    float des_period = (((float)period_ms) / ((float)1000.0f));

    load = (uint32_t)(des_period / ((float)cpu_period * ((float)(prescaler + 1U))));
    if(max_val < load) {
        res = false;
    } else {
        res = true;
    }
    if(res) {
        calc_period = tim_calc_real_period_s(cpu_clock, prescaler, (uint32_t)load);
        if(false == is_float_equal_absolute(calc_period, des_period, cpu_period * 32.0f)) {
            LOG_WARNING(TIM, "Periods different des %7.4f  calc %7.4f s", des_period, calc_period);
            res = false;
        }
        res = true;
        *out_load = (uint32_t)load;
    }

    if(false == res) {
        *out_load = 0xFFFF - 1;
        res = false;
    }

    return res;
}

#if 0
static bool tim_init_item(uint32_t index, uint32_t period_ms, uint8_t cnt_period_us) {
    bool res = false;

    return res;
}
#endif

bool tim_init(void) {
    bool res = false;

    return res;
}

uint8_t tim_get_width(uint32_t tim_base) {
    uint8_t width = 0;

    return width;
}
