#include "clocks.h"

#include <stdbool.h>
#include <stdint.h>
#include <systick.h>

#include "sys_tick.h"

bool clock_init(void){
    return true;
}

bool delay_ms(uint32_t delay_in_ms) {
    bool res = false;
    uint32_t init_time_in_ms = g_up_time_ms;
    while(g_up_time_ms < (init_time_in_ms + delay_in_ms)) {
        res = true;
    }
    return res;
}

// uint32_t getRunTimeCounterValue32(void) { return (uint32_t)g_up_time_ms; }
// overflow after 4294967 s 49 days
uint64_t diff_us = 0;
uint64_t get_time_us(void) {
    uint64_t up_time_ms;
    uint32_t usec;
    uint32_t cnt;
    uint32_t sys_tick_val;
    /*Sys tick counts down (Wrap to zero counter)*/
    static uint64_t prev_time_us = 0;
    static uint64_t cur_time_us = 0;
    sys_tick_val = SysTickValueGet();
    up_time_ms = g_up_time_ms;
    cnt = SYS_TICK_PERIOD - sys_tick_val;
    usec = cnt / CLOCK_FOR_US;
    prev_time_us = cur_time_us;
    cur_time_us = ((((uint64_t)up_time_ms) * ((uint64_t)1000UL)) + ((uint64_t)usec));
    if(cur_time_us < prev_time_us) { /*Error*/
        diff_us = prev_time_us - cur_time_us;
    }
    return cur_time_us;
}

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
uint64_t pause_1ms(void) {
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

uint32_t clock_incr_per_ms(uint32_t delay_ms) {
    uint32_t cnt = 0;
    uint32_t start_time_ms = get_time_ms32();
    uint32_t cut_time_ms = 0;
    uint32_t diff_ms = 0;
    while(1) {
        cut_time_ms = get_time_ms32();
        diff_ms = cut_time_ms - start_time_ms;
        if(delay_ms < diff_ms) {
            break;
        }
        cnt++;
    }
    return cnt;
}
