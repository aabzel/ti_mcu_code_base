/*
https://software-dl.ti.com/dsps/dsps_public_sw/sdo_sb/targetcontent/tirtos/2_20_00_06/exports/tirtos_full_2_20_00_06/products/tidrivers_full_2_20_00_08/docs/doxygen/html/_g_p_timer_c_c26_x_x_8h.html
 */
#include "tim_drv.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <timer.h>

#ifdef DeviceFamily_CC26X2
#include <ti/drivers/timer/GPTimerCC26XX.h>
#endif

#ifdef HAS_RTC
#include "rtc_drv.h"
#endif

#include "bit_utils.h"
#include "clocks.h"
#include "float_utils.h"
#ifdef HAS_LOG
#include "log.h"
#endif
#include "sys_config.h"

Timer_t TimerItem[BOARD_GPTIMERPARTSCOUNT] = {{.hTimer = NULL, .tim_it_cnt = 0, .cnt_period_us = 1, .period_ms = 89478},
                                              {.hTimer = NULL, .tim_it_cnt = 0, .cnt_period_us = 1, .period_ms = 2},
                                              {.hTimer = NULL, .tim_it_cnt = 0, .cnt_period_us = 1, .period_ms = 1000U},
                                              {.hTimer = NULL, .tim_it_cnt = 0, .cnt_period_us = 1, .period_ms = 100},
                                              {.hTimer = NULL, .tim_it_cnt = 0, .cnt_period_us = 5, .period_ms = 1000},
                                              {.hTimer = NULL, .tim_it_cnt = 0, .cnt_period_us = 1, .period_ms = 500},
                                              {.hTimer = NULL, .tim_it_cnt = 0, .cnt_period_us = 1, .period_ms = 100},
                                              {.hTimer = NULL, .tim_it_cnt = 0, .cnt_period_us = 1, .period_ms = 100}};

GPTimerCC26XX_Handle hTimer;
uint32_t TimInstLUT[3] = {TIMER_A, TIMER_B, TIMER_BOTH};
uint32_t TimBaseLut[4] = {GPT0_BASE, GPT1_BASE, GPT2_BASE, GPT3_BASE};

/*
 *  =============================== GPTimer ===============================
 *  Remove unused entries to reduce flash usage both in Board.c and Board.h
 */
#include <ti/drivers/Power.h>
#include <ti/drivers/power/PowerCC26X2.h>
#include <ti/drivers/timer/GPTimerCC26XX.h>

GPTimerCC26XX_Object gptimerCC26XXObjects[BOARD_GPTIMERCOUNT];

const GPTimerCC26XX_HWAttrs gptimerCC26xxHWAttrs[BOARD_GPTIMERPARTSCOUNT] = {
    {
        .baseAddr = GPT0_BASE,
        .intNum = INT_GPT0A,
        .intPriority = (~0),
        .powerMngrId = PowerCC26XX_PERIPH_GPT0,
        .pinMux = GPT_PIN_0A,
    },
    {
        .baseAddr = GPT0_BASE,
        .intNum = INT_GPT0B,
        .intPriority = (~0),
        .powerMngrId = PowerCC26XX_PERIPH_GPT0,
        .pinMux = GPT_PIN_0B,
    },
    {
        .baseAddr = GPT1_BASE,
        .intNum = INT_GPT1A,
        .intPriority = (~0),
        .powerMngrId = PowerCC26XX_PERIPH_GPT1,
        .pinMux = GPT_PIN_1A,
    },
    {
        .baseAddr = GPT1_BASE,
        .intNum = INT_GPT1B,
        .intPriority = (~0),
        .powerMngrId = PowerCC26XX_PERIPH_GPT1,
        .pinMux = GPT_PIN_1B,
    },
    {
        .baseAddr = GPT2_BASE,
        .intNum = INT_GPT2A,
        .intPriority = (~0),
        .powerMngrId = PowerCC26XX_PERIPH_GPT2,
        .pinMux = GPT_PIN_2A,
    },
    {
        .baseAddr = GPT2_BASE,
        .intNum = INT_GPT2B,
        .intPriority = (~0),
        .powerMngrId = PowerCC26XX_PERIPH_GPT2,
        .pinMux = GPT_PIN_2B,
    },
    {
        .baseAddr = GPT3_BASE,
        .intNum = INT_GPT3A,
        .intPriority = (~0),
        .powerMngrId = PowerCC26XX_PERIPH_GPT3,
        .pinMux = GPT_PIN_3A,
    },
    {
        .baseAddr = GPT3_BASE,
        .intNum = INT_GPT3B,
        .intPriority = (~0),
        .powerMngrId = PowerCC26XX_PERIPH_GPT3,
        .pinMux = GPT_PIN_3B,
    },
};

const GPTimerCC26XX_Config GPTimerCC26XX_config[BOARD_GPTIMERPARTSCOUNT] = {
    {&gptimerCC26XXObjects[BOARD_GPTIMER0], &gptimerCC26xxHWAttrs[BOARD_GPTIMER0A], GPT_A},
    {&gptimerCC26XXObjects[BOARD_GPTIMER0], &gptimerCC26xxHWAttrs[BOARD_GPTIMER0B], GPT_B},
    {&gptimerCC26XXObjects[BOARD_GPTIMER1], &gptimerCC26xxHWAttrs[BOARD_GPTIMER1A], GPT_A},
    {&gptimerCC26XXObjects[BOARD_GPTIMER1], &gptimerCC26xxHWAttrs[BOARD_GPTIMER1B], GPT_B},
    {&gptimerCC26XXObjects[BOARD_GPTIMER2], &gptimerCC26xxHWAttrs[BOARD_GPTIMER2A], GPT_A},
    {&gptimerCC26XXObjects[BOARD_GPTIMER2], &gptimerCC26xxHWAttrs[BOARD_GPTIMER2B], GPT_B},
    {&gptimerCC26XXObjects[BOARD_GPTIMER3], &gptimerCC26xxHWAttrs[BOARD_GPTIMER3A], GPT_A},
    {&gptimerCC26XXObjects[BOARD_GPTIMER3], &gptimerCC26xxHWAttrs[BOARD_GPTIMER3B], GPT_B},
};

void timerCallback0(GPTimerCC26XX_Handle handle, GPTimerCC26XX_IntMask interruptMask) {
    // interrupt callback code goes here. Minimize processing in interrupt.
    if(TimerItem[0].hTimer == handle) {
        TimerItem[0].tim_it_cnt++;
    }
}

void timerCallback1(GPTimerCC26XX_Handle handle, GPTimerCC26XX_IntMask interruptMask) {
    // interrupt callback code goes here. Minimize processing in interrupt.
    if(TimerItem[1].hTimer == handle) {
        TimerItem[1].tim_it_cnt++;
    }
}

void timerCallback2(GPTimerCC26XX_Handle handle, GPTimerCC26XX_IntMask interruptMask) {
    // interrupt callback code goes here. Minimize processing in interrupt.
    if(TimerItem[2].hTimer == handle) {
        TimerItem[2].tim_it_cnt++;
#ifdef HAS_RTC
        SwRtc.raw_sec++;
#endif
    }
}

void timerCallback3(GPTimerCC26XX_Handle handle, GPTimerCC26XX_IntMask interruptMask) {
    // interrupt callback code goes here. Minimize processing in interrupt.
    if(TimerItem[3].hTimer == handle) {
        TimerItem[3].tim_it_cnt++;
    }
}

void timerCallback4(GPTimerCC26XX_Handle handle, GPTimerCC26XX_IntMask interruptMask) {
    // interrupt callback code goes here. Minimize processing in interrupt.
    if(TimerItem[4].hTimer == handle) {
        TimerItem[4].tim_it_cnt++;
    }
}

void timerCallback5(GPTimerCC26XX_Handle handle, GPTimerCC26XX_IntMask interruptMask) {
    // interrupt callback code goes here. Minimize processing in interrupt.
    if(TimerItem[5].hTimer == handle) {
        TimerItem[5].tim_it_cnt++;
    }
}

void timerCallback6(GPTimerCC26XX_Handle handle, GPTimerCC26XX_IntMask interruptMask) {
    // interrupt callback code goes here. Minimize processing in interrupt.
    if(TimerItem[6].hTimer == handle) {
        TimerItem[6].tim_it_cnt++;
    }
}

void timerCallback7(GPTimerCC26XX_Handle handle, GPTimerCC26XX_IntMask interruptMask) {
    // interrupt callback code goes here. Minimize processing in interrupt.
    if(TimerItem[7].hTimer == handle) {
        TimerItem[7].tim_it_cnt++;
    }
}

void (*timerCallback[BOARD_GPTIMERPARTSCOUNT])(GPTimerCC26XX_Handle handle, GPTimerCC26XX_IntMask interruptMask) = {
    timerCallback0, timerCallback1, timerCallback2, timerCallback3,
    timerCallback4, timerCallback5, timerCallback6, timerCallback7};

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
#ifdef HAS_LOG
            LOG_WARNING(TIM, "Periods different des %7.4f  calc %7.4f s", des_period, calc_period);
#endif
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

static bool tim_init_item(uint32_t index, uint32_t period_ms, uint8_t cnt_period_us) {
    bool res = false;
    TimerItem[index].tim_it_cnt = 0;
    GPTimerCC26XX_Params params;
    GPTimerCC26XX_Params_init(&params);
    params.width = GPT_CONFIG_32BIT;
    params.mode = GPT_MODE_PERIODIC_UP;
    params.direction = GPTimerCC26XX_DIRECTION_UP;
    params.debugStallMode = GPTimerCC26XX_DEBUG_STALL_OFF;
    TimerItem[index].hTimer = GPTimerCC26XX_open(index, &params);
    if(NULL == TimerItem[index].hTimer) {
        res = false;
    } else {
        res = true;
    }

    if(res) {
        uint32_t prescaler = 0;
        GPTimerCC26XX_Value load_val = 0;
        prescaler = 0;
#ifdef HAS_TIM16BIT
        if(TIM_MAX_PSC < prescaler) {
            LOG_INFO(TIM, "TIM%u TooBigPsc %u", index, prescaler);
            prescaler = TIM_MAX_PSC;
        }
        LOG_INFO(TIM, "TIM%u SetPrescaler %u", index, prescaler);
#endif
#ifdef HAS_LOG
        LOG_INFO(TIM, "TIM%u DesPrescaler %u", index, prescaler);
#endif
        res = tim_calc_registers(period_ms, SYS_FREQ, prescaler, &load_val, 0xFFFFFFFE);
        if(res) {
#ifdef HAS_LOG
            LOG_INFO(TIM, "TIM%u SetLoad %u", index, load_val);
#endif
            GPTimerCC26XX_setLoadValue(TimerItem[index].hTimer, load_val);
            // TimerPrescaleSet(gptimerCC26xxHWAttrs[index].baseAddr, TimInstLUT[index % 2], prescaler-1);
            // TimerPrescaleMatchSet(gptimerCC26xxHWAttrs[index].baseAddr, TimInstLUT[index % 2], prescaler-1);
            GPTimerCC26XX_registerInterrupt(TimerItem[index].hTimer, timerCallback[index], GPT_INT_TIMEOUT);
            GPTimerCC26XX_start(TimerItem[index].hTimer);
            // TimerPrescaleSet(gptimerCC26xxHWAttrs[index].baseAddr, TimInstLUT[index % 2], prescaler-1);
            // TimerEnable(uint32_t ui32Base, uint32_t ui32Timer);
            // (TimBaseLut[tim_base_id], TimInstLUT[part]);
            // TimerPrescaleMatchSet(gptimerCC26xxHWAttrs[index].baseAddr, TimInstLUT[index % 2], prescaler-1);
        } else {
#ifdef HAS_LOG
            LOG_ERROR(TIM, "Unable to set timer %u", index);
#endif
            res = false;
        }
    }

    return res;
}

bool tim_init(void) {
    bool res = true;
    uint8_t tim_num;
    for(tim_num = 0; tim_num < BOARD_GPTIMERPARTSCOUNT; tim_num += 2) {
        res = tim_init_item(tim_num, TimerItem[tim_num].period_ms, TimerItem[tim_num].cnt_period_us) && res;
    }
    return res;
}

uint8_t tim_get_width(uint32_t tim_base) {
    uint32_t cfg_reg = 0;
    uint8_t width = 0;
    cfg_reg = HWREG(tim_base + GPT_O_CFG);
    switch(MASK_2BIT & cfg_reg) {
    case 0:
        width = 32;
        break;
    case 4:
        width = 16;
        break;
    default:
        break;
    }
    return width;
}

uint32_t tim_get_us(void) {
    uint32_t time_us = 0;
    time_us = TimerValueGet(GPT0_BASE, TIMER_A);
    time_us = time_us / CLOCK_FOR_US;
    return time_us;
}

uint32_t tim_get_ms(void) {
    uint32_t time_ms = 0;
    time_ms = TimerValueGet(GPT0_BASE, TIMER_A);
    time_ms = time_ms / (CLOCK_FOR_US * 1000);
    return time_ms;
}
