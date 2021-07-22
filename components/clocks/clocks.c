#include "clocks.h"

#include <stdbool.h>
#include <stdint.h>


volatile uint32_t g_up_time_ms = 0;



uint32_t HAL_GetTick(void) { return g_up_time_ms; }


void HAL_IncTick(void) { g_up_time_ms++; }

void delay_ms(uint32_t delay_in_ms) {
    uint32_t init_time_in_ms = g_up_time_ms;
    while (g_up_time_ms < (init_time_in_ms + delay_in_ms)) {
    }
}

uint32_t getRunTimeCounterValue32(void) { return (uint32_t)g_up_time_ms; }

uint64_t getRunTimeCounterValue64(void) { return g_up_time_ms * 1000; }

uint32_t get_time_ms32(void) { return g_up_time_ms; }

uint64_t get_time_ms64(void) { return g_up_time_ms; }

uint64_t get_time_us(void) { return g_up_time_ms * 1000; }
