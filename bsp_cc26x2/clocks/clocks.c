#include "clocks.h"

#include <stdbool.h>
#include <stdint.h>

#include "sys_tick.h"

uint32_t clock_get_tick_ms(void) { return g_up_time_ms; }

void delay_ms(uint32_t delay_in_ms) {
    uint32_t init_time_in_ms = g_up_time_ms;
    while(g_up_time_ms < (init_time_in_ms + delay_in_ms)) {
    }
}

uint32_t getRunTimeCounterValue32(void) { return (uint32_t)g_up_time_ms; }

uint64_t getRunTimeCounterValue64(void) {
    uint64_t up_time_us = 0;
    up_time_us = ((uint64_t)g_up_time_ms) * ((uint64_t)1000U);
    return up_time_us;
}

uint32_t get_time_ms32(void) { return g_up_time_ms; }

uint64_t get_time_ms64(void) { return (uint64_t)g_up_time_ms; }

uint64_t get_time_us(void) { return g_up_time_ms * 1000; }

#if 0
/*calibrated*/
static uint64_t pause_1s(void) {
    uint64_t in = 0, cnt = 0;
    for(in = 0; in < 1319406; in++) {
        cnt++;
    }
    return cnt;
}
#endif

/*calibrated*/
static uint64_t pause_1ms(void) {
    uint64_t in = 0, cnt = 0;
    for(in = 0; in < 1397; in++) {
        cnt++;
    }
    return cnt;
}

uint64_t sw_pause_ms(uint32_t delay_in_ms) {
    uint64_t cnt = 0;
    uint32_t t = 0;
    for(t = 0; t < delay_in_ms; t++) {
        cnt += pause_1ms();
    }
    return cnt;
}
