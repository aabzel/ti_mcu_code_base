#include "clocks.h"

#include <stdbool.h>
#include <stdint.h>
#include <systick.h>

#include "sys_tick.h"

uint32_t clock_get_tick_ms(void) { return g_up_time_ms; }

void delay_ms(uint32_t delay_in_ms) {
    uint32_t init_time_in_ms = g_up_time_ms;
    while(g_up_time_ms < (init_time_in_ms + delay_in_ms)) {
    }
}

// uint32_t getRunTimeCounterValue32(void) { return (uint32_t)g_up_time_ms; }
// overflow after 4294967 s 49 days
uint64_t get_time_us(void) {
    /*Sys tick counts down. Wrap to zero counter*/
    uint64_t usec = (((uint64_t)SYS_TICK_MAX_VAL) - ((uint64_t)SysTickValueGet())) / ((uint64_t)CLOCK_FOR_US);
    return ((((uint64_t)g_up_time_ms) * ((uint64_t)1000UL)) + usec);
}

// uint64_t getRunTimeCounterValue64(void) {
//    return get_time_us();
//}

uint32_t get_time_ms32(void) { return g_up_time_ms; }

uint64_t get_time_ms64(void) { return (uint64_t)g_up_time_ms; }

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

uint64_t pause_1us(void) {
    uint64_t in = 0, cnt = 0;
    for(in = 0; in < 297; in++) {
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
