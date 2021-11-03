#ifndef CLOCKS_H
#define CLOCKS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#define SYS_FREQ 48000000U
#define CLOCK_FOR_US (SYS_FREQ / 1000000U)

#define COUNTER_FREQ (SYS_FREQ / 1000000U)

#define US_TO_COUNTER(us) (uint64_t)((uint64_t)(us) * (COUNTER_FREQ))
#define MS_TO_COUNTER(ms) (uint64_t)(((ms)*1000UL) * (COUNTER_FREQ))
#define COUNTER_TO_US(counter) ((counter) / (COUNTER_FREQ))
#define COUNTER_TO_MS(counter) ((counter) / ((COUNTER_FREQ)*1000))

#define HOUR_2_MS(HOUR) ((HOUR)*60U * 60U * 1000U)
#define MIN_2_MS(MIN) ((MIN)*60U * 1000U)
#define S_2_MS(S) ((S)*1000U)
#define S_2_US(S) ((S)*1000000U)
#define MS_2_S(ms) ((((double)ms) / 1000.0))
#define MS_2_MIN(ms) (ms / (1000 * 60))

uint32_t get_time_ms32(void);
uint64_t get_time_ms64(void);
uint64_t get_time_us(void);
uint64_t get_time_tick64(void);
uint64_t tick2us(uint64_t tick);
uint64_t getRunTimeCounterValue64(void);
uint32_t getRunTimeCounterValue32(void);
uint64_t us2tick(uint64_t ms64);
uint64_t get_runtime_counter(void);
uint64_t runtime_2_us(uint64_t rtc);
uint64_t pause_1ms(void);
uint64_t pause_1us(void);
bool delay_ms(uint32_t delay_in_ms);
void delay_us(uint32_t delay_in_us);
uint32_t clock_incr_per_ms(uint32_t delay_ms);
uint64_t sw_pause_ms(uint32_t delay_in_ms);
// uint32_t clock_get_tick_ms(void);
uint32_t clock_incr_per_ms(uint32_t delay_ms);
bool SystemClock_Config(void);

#ifdef __cplusplus
}
#endif

#endif // CLOCKS_H
