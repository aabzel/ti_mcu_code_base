#ifndef TIMER_UTILS_H_
#define TIMER_UTILS_H_
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

extern uint32_t get_time_ms32(void);
extern uint64_t get_time_ms64(void);
uint64_t get_time_us(void);
uint64_t get_time_tick64(void);
uint64_t tick2us(uint64_t tick);
uint64_t us2tick(uint64_t ms64);

uint64_t get_runtime_counter(void);
uint64_t runtime_2_us(uint64_t rtc);

#ifndef MATLAB_MEX_FILE
void delay(uint32_t delay_in_ms);
void delay_wd(uint32_t delay_in_ms);
void delay_us(uint32_t delay_in_us);
void delay_us_wd(uint32_t delay_in_us);
#endif /* MATLAB_MEX_FILE */

#ifdef __cplusplus
}
#endif

#endif /* TIMER_UTILS_H_ */
