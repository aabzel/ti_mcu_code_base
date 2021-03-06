#ifndef TIM_DRV_H
#define TIM_DRV_H

#include <stdbool.h>
#include <stdint.h>

#include <ti/drivers/timer/GPTimerCC26XX.h>

#define TIM_MAX_PSC 255
#define XTALL_TICKS_IN_1US (SYS_FREQ/1000000)
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
    uint32_t period_ms;
    uint8_t cnt_period_us; /* max 5.3125us*/
} Timer_t;

extern Timer_t TimerItem[BOARD_GPTIMERPARTSCOUNT];
extern const GPTimerCC26XX_HWAttrs gptimerCC26xxHWAttrs[BOARD_GPTIMERPARTSCOUNT];

extern uint32_t TimInstLUT[3];
extern uint32_t TimBaseLut[4];

uint8_t tim_get_width(uint32_t tim_base);
bool tim_init(void);
float tim_calc_real_period_s(uint32_t cpu_clock, uint32_t prescaler, uint32_t laod);
bool tim_calc_registers(uint32_t period_ms, uint32_t cpu_clock, uint32_t prescaler, uint32_t* out_load,
                        uint32_t max_val);

uint32_t tim_get_us(void);
uint32_t tim_get_ms(void);

#endif /* TIM_DRV_H  */
