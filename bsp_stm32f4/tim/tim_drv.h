#ifndef TIM_DRV_H
#define TIM_DRV_H

#include <stdbool.h>
#include <stdint.h>


typedef struct xTimer_t {
    uint32_t tim_it_cnt;
    uint32_t period_ms;
    uint8_t cnt_period_us; /* 5.3125e-06 s*/
} Timer_t;


extern Timer_t TimerItem[14];

uint8_t tim_get_width(uint32_t tim_base);
bool tim_init(void);
float tim_calc_real_period_s(uint32_t cpu_clock, uint32_t prescaler, uint32_t laod);
bool tim_calc_registers(uint32_t period_ms, uint32_t cpu_clock, uint32_t prescaler, uint32_t* out_load,
                        uint32_t max_val);

#endif /* TIM_DRV_H  */
