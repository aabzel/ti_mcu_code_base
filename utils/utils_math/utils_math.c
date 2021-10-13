#include "utils_math.h"

#include <math.h>
#include <stdbool.h>
#include <stdint.h>

uint8_t pwm_sample_calc(uint32_t cur_time_ms, float freq, uint32_t cur_phase_ms, float duty_cycle){
    float line_val = 0.0f;
    float saw_val = 0.0f;

    if(100.0f < duty_cycle) {
        duty_cycle = 100.0f;
    }
    if(duty_cycle < 0.0f) {
        duty_cycle = 0.0f;
    }

    line_val = ((((float)cur_time_ms) + ((float)cur_phase_ms)) / 1000.0f) * freq; /*tune frequency*/
    saw_val = fmodf(line_val, 1.0f);                            /*tune DutyCycle*/

    uint8_t logic_level = 0U;
    float val = (((1.0f - ((100.0f - duty_cycle)) / 100.0f)) - saw_val); /*tune DutyCycle*/
    if(0.0f < val) {
        logic_level = 1U;
    }else{
        logic_level = 0U;
    }

    return logic_level;
}

uint8_t pwm_sample_calc_num(uint32_t cut_tick_ms, uint32_t period_ms, uint8_t duty, int32_t phase_ms) {
    uint8_t val = 0;
    if(100 < duty) {
        duty = 100;
    }
    int32_t time_saw = (phase_ms + ((int32_t)cut_tick_ms)) % period_ms;
    if ((((period_ms * duty) / 100)) < time_saw) {
        val = 1;
    } else {
        val = 0;
    }

    return val;
}

