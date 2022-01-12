#ifndef CLOCKS_H
#define CLOCKS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "sys_config.h"


#define COUNTER_FREQ (SYS_FREQ / 1000000U)

#define US_TO_COUNTER(us) (uint64_t)((uint64_t)(us) * (COUNTER_FREQ))
#define MS_TO_COUNTER(ms) (uint64_t)(((ms)*1000UL) * (COUNTER_FREQ))

#define HOUR_2_MS(HOUR) ((HOUR)*60U * 60U * 1000U)
#define MIN_2_MS(MIN) ((MIN)*60U * 1000U)
#define S_2_MS(S) ((S)*1000U)
#define S_2_US(SEC) ((uint32_t)(1000000.0 * ((float)SEC)))
#define MS_2_S(ms) ((((double)ms) / 1000.0))
#define MS_2_MIN(ms) (ms / (1000 * 60))

bool clock_init(void);
uint32_t get_time_ms32(void);
uint64_t get_time_ms64(void);
uint64_t get_time_us(void);
bool delay_ms(uint32_t delay_in_ms);
void delay_us(uint32_t delay_in_us);

#ifdef __cplusplus
}
#endif

#endif // CLOCKS_H
