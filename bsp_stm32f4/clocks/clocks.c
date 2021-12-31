#include "clocks.h"

#include <stdbool.h>
#include <stdint.h>

#include "stm32f4xx_hal.h"
#include "sys_tick.h"

bool delay_ms(uint32_t delay_in_ms) {
    bool res = false;
    uint32_t init_time_in_ms = g_up_time_ms;
    while(g_up_time_ms < (init_time_in_ms + delay_in_ms)) {
        res = true;
    }
    return res;
}

// overflow after 4294967 s 49 days
uint64_t diff_us = 0;
uint64_t get_time_us(void) {
    uint64_t up_time_ms;
    uint32_t usec;
    uint32_t cnt;
    uint32_t sys_tick_val = 0;
    /*Sys tick counts down (Wrap to zero counter)*/
    static uint64_t prev_time_us = 0;
    static uint64_t cur_time_us = 0;
    sys_tick_val = SysTick->VAL;
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
/*calibrate*/
static uint64_t pause_1s(void) {
    uint64_t in = 0, cnt = 0;
    for(in = 0; in < 1319406; in++) {
        cnt++;
    }
    return cnt;
}
#endif

/*calibrate*/
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

bool SystemClockConfig(void) {
    bool res = true;
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Configure the main internal regulator output voltage
     */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM = 12;
    RCC_OscInitStruct.PLL.PLLN = 180;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 7;
    if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        res = false;
    }
    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK) {
        res = false;
    }
    return res;
}
