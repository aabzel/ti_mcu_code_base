#ifndef TIM_DRV_H
#define TIM_DRV_H

#include <stdbool.h>
#include <stdint.h>

#include <ti/drivers/timer/GPTimerCC26XX.h>
/*!
 *  @def    BOARD_GPTimerName
 *  @brief  Enum of GPTimer parts
 */
typedef enum BOARD_GPTimerName {
    BOARD_GPTIMER0A = 0,
    BOARD_GPTIMER0B = 1,
    BOARD_GPTIMER1A = 2,
    BOARD_GPTIMER1B = 3,
    BOARD_GPTIMER2A = 4,
    BOARD_GPTIMER2B = 5,
    BOARD_GPTIMER3A = 6,
    BOARD_GPTIMER3B = 7,

    BOARD_GPTIMERPARTSCOUNT
} BOARD_GPTimerName;

/*!
 *  @def    BOARD_GPTimers
 *  @brief  Enum of GPTimers
 */
typedef enum BOARD_GPTimers {
    BOARD_GPTIMER0 = 0,
    BOARD_GPTIMER1 = 1,
    BOARD_GPTIMER2 = 2,
    BOARD_GPTIMER3 = 3,

    BOARD_GPTIMERCOUNT = 4
} BOARD_GPTimers;

typedef struct xTimer_t {
    GPTimerCC26XX_Handle hTimer;
    uint32_t tim_it_cnt;
    uint32_t pesiod_ms;
} Timer_t;

extern Timer_t TimerItem[BOARD_GPTIMERPARTSCOUNT];

extern uint32_t TimInstLUT[2];
extern uint32_t TimBaseLut[4];

bool tim_init(void);
float tim_calc_real_period_s(uint32_t cpu_clock, uint32_t prescaler, uint32_t laod);
bool tim_calc_registers(uint32_t pesiod_ms, uint32_t cpu_clock, uint32_t* out_prescaler, uint32_t* out_load);

#endif /* TIM_DRV_H  */
