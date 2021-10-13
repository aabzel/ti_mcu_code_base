#ifndef UTILS_MATH_H
#define UTILS_MATH_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

uint8_t pwm_sample_calc_num(uint32_t cut_tick_ms, uint32_t period_ms, uint8_t duty, int32_t phase_ms);
uint8_t pwm_sample_calc(uint32_t cur_time_ms, float freq, uint32_t phase_ms, float duty_cycle) ;

#ifdef __cplusplus
}
#endif

#endif /* UTILS_MATH_H */
