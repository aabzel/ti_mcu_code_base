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

#include "clocks.h"
#include "float_utils.h"

Timer_t TimerItem[BOARD_GPTIMERPARTSCOUNT] = {
    {.hTimer = NULL, .tim_it_cnt = 0, .pesiod_ms = 1},
    {.hTimer = NULL, .tim_it_cnt = 0, .pesiod_ms = 2},
    {.hTimer = NULL, .tim_it_cnt = 0, .pesiod_ms = 10},
    {.hTimer = NULL, .tim_it_cnt = 0, .pesiod_ms = 100},
    {.hTimer = NULL, .tim_it_cnt = 0, .pesiod_ms = 1000},
    {.hTimer = NULL, .tim_it_cnt = 0, .pesiod_ms = 500},
    {.hTimer = NULL, .tim_it_cnt = 0, .pesiod_ms = 100},
    {.hTimer = NULL, .tim_it_cnt = 0, .pesiod_ms = 100}};

GPTimerCC26XX_Handle hTimer;
uint32_t TimInstLUT[2] = {TIMER_A, TIMER_B};
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
    if(TimerItem[0].hTimer ==handle ){
        TimerItem[0].tim_it_cnt++;
    }
    if(TimerItem[1].hTimer ==handle ){
        TimerItem[1].tim_it_cnt++;
    }
}

void timerCallback1(GPTimerCC26XX_Handle handle, GPTimerCC26XX_IntMask interruptMask) {
    // interrupt callback code goes here. Minimize processing in interrupt.
    if(TimerItem[0].hTimer ==handle ){
        TimerItem[0].tim_it_cnt++;
    }
    if(TimerItem[1].hTimer ==handle ){
        TimerItem[1].tim_it_cnt++;
    }
}

void timerCallback2(GPTimerCC26XX_Handle handle, GPTimerCC26XX_IntMask interruptMask) {
    // interrupt callback code goes here. Minimize processing in interrupt.
    if(TimerItem[2].hTimer ==handle ){
        TimerItem[2].tim_it_cnt++;
    }
}

void timerCallback3(GPTimerCC26XX_Handle handle, GPTimerCC26XX_IntMask interruptMask) {
    // interrupt callback code goes here. Minimize processing in interrupt.
    if(TimerItem[3].hTimer ==handle ){
        TimerItem[3].tim_it_cnt++;
    }
}

void timerCallback4(GPTimerCC26XX_Handle handle, GPTimerCC26XX_IntMask interruptMask) {
    // interrupt callback code goes here. Minimize processing in interrupt.
    if(TimerItem[4].hTimer ==handle ){
        TimerItem[4].tim_it_cnt++;
    }
}

void timerCallback5(GPTimerCC26XX_Handle handle, GPTimerCC26XX_IntMask interruptMask) {
    // interrupt callback code goes here. Minimize processing in interrupt.
    if(TimerItem[5].hTimer ==handle ){
        TimerItem[5].tim_it_cnt++;
    }
}

void timerCallback6(GPTimerCC26XX_Handle handle, GPTimerCC26XX_IntMask interruptMask) {
    // interrupt callback code goes here. Minimize processing in interrupt.
    if(TimerItem[6].hTimer ==handle ){
        TimerItem[6].tim_it_cnt++;
    }
}

void timerCallback7(GPTimerCC26XX_Handle handle, GPTimerCC26XX_IntMask interruptMask) {
    // interrupt callback code goes here. Minimize processing in interrupt.
    if(TimerItem[7].hTimer ==handle ){
        TimerItem[7].tim_it_cnt++;
    }
}

void (*timerCallback[BOARD_GPTIMERPARTSCOUNT])(GPTimerCC26XX_Handle handle, GPTimerCC26XX_IntMask interruptMask) = {
    timerCallback0, timerCallback1, timerCallback2, timerCallback3,
    timerCallback4, timerCallback5, timerCallback6, timerCallback7};

float tim_calc_real_period_s(uint32_t cpu_clock, uint32_t prescaler, uint32_t laod) {
    float calc_period = 0.0f;
    float cpu_period = 1.0f / cpu_clock;
    calc_period = cpu_period * ((float)((prescaler + 1U) * laod));
    return calc_period;
}

bool tim_calc_registers(uint32_t pesiod_ms, uint32_t cpu_clock, uint32_t* out_prescaler, uint32_t* out_load) {
    bool res = false;
    uint32_t prescaler = 0;
    uint32_t load = 0;
    bool loop = true;
    float cpu_period = 1.0f / cpu_clock;
    float calc_period = 0;
    float des_period = (((float)pesiod_ms) / ((float)1000.0f));
    for(prescaler = 0; prescaler <= 0xFF; prescaler++) {
        load = (uint32_t)(des_period / ((float)cpu_period * ((float)(prescaler + 1U))));
        if(0xFFFF<load){
            continue;
        }
        calc_period = tim_calc_real_period_s(cpu_clock, prescaler, load);
        if(true == is_float_equal_absolute(calc_period, des_period, cpu_period * 2)) {
            *out_prescaler = prescaler;
            *out_load = load;
            res = true;
            loop = false;
        }
        if(false == loop) {
            break;
        }
        if(des_period < calc_period) {
            loop = false;
        }
    }

    if(false==res){
        *out_prescaler = 0xFF;
        *out_load = 0xFFFF-1;
        res = true;
    }

    return res;
}

static bool tim_init_item(uint32_t index) {
    bool res = false;
    TimerItem[index].tim_it_cnt = 0;
    GPTimerCC26XX_Params params;
    GPTimerCC26XX_Params_init(&params);
    params.width = GPT_CONFIG_16BIT;
    params.mode = GPT_MODE_PERIODIC_UP;
    params.debugStallMode = GPTimerCC26XX_DEBUG_STALL_OFF;
    TimerItem[index].hTimer = GPTimerCC26XX_open(index, &params);
    if(NULL == TimerItem[index].hTimer) {
        res = false;
    } else {
        res = true;
    }


    uint32_t prescaler = 0;
    GPTimerCC26XX_Value load_val = 0; // 47999
    if(res) {
        prescaler = 0;
        res = tim_calc_registers(TimerItem[index].pesiod_ms, SYS_FREQ, &prescaler, &load_val);
        if (res) {
          GPTimerCC26XX_setLoadValue(TimerItem[index].hTimer, load_val);
          TimerPrescaleSet(gptimerCC26xxHWAttrs[index].baseAddr, TimInstLUT[index%2],   prescaler);
          TimerPrescaleMatchSet(gptimerCC26xxHWAttrs[index].baseAddr, TimInstLUT[index%2],   prescaler);
          GPTimerCC26XX_registerInterrupt(TimerItem[index].hTimer, timerCallback[index], GPT_INT_TIMEOUT);
          GPTimerCC26XX_start(TimerItem[index].hTimer);
          TimerPrescaleSet(gptimerCC26xxHWAttrs[index].baseAddr, TimInstLUT[index%2],   prescaler);
          TimerPrescaleMatchSet(gptimerCC26xxHWAttrs[index].baseAddr, TimInstLUT[index%2],   prescaler);
        }else{
            res = false;
        }
    }

    return res;
}

bool tim_init(void) {
    bool res = true;
    uint8_t tim_num;
    for(tim_num = 0; tim_num < BOARD_GPTIMERPARTSCOUNT; tim_num++) {
        res = tim_init_item(tim_num) && res;
    }
    return res;
}