#ifndef TIMER_UTILS_H
#define TIMER_UTILS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SEC_2_US(SEC) ((SEC)*1000000U)
#define MIN_2_US(MIN) ((uint32_t)(60000000 * (MIN)))
#define MSEC_2_US(MSEC) ((MSEC)*1000U)
#define MSEC_2_S(MSEC) (((double)MSEC)/1000.0)
#define MSEC_2_MIN(MSEC) ( (((double)MSEC/1000.0)/60.0) )
#define MSEC_2_H(MSEC) ( ((((double)MSEC/1000.0)/60.0)/60.0) )
#define HOUR_2_MS(HOUR) ((HOUR)*60U * 60U * 1000U)
#define MIN_2_MS(MIN) ((MIN)*60U * 1000U)
#define S_2_MS(S) ((S)*1000U)
#define S_2_US(SEC) ((uint32_t)(1000000.0 * ((float)SEC)))
#define MS_2_S(ms) ((((double)ms) / 1000.0))
#define MS_2_MIN(ms) (((double)ms) / ((double)(1000 * 60)))

uint64_t get_runtime_counter(void);
uint64_t get_time_tick64(void);

uint64_t runtime_2_us(uint64_t rtc);
uint64_t tick2us(uint64_t tick);
uint64_t us2tick(uint64_t ms64);
void delay_us(uint32_t delay_in_us);
void delay_us_wd(uint32_t delay_in_us);
void delay_wd(uint32_t delay_in_ms);

#ifdef __cplusplus
}
#endif

#endif /* TIMER_UTILS_H */
