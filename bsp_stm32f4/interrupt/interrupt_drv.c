#include "interrupt_drv.h"

#include <stdint.h>

#include "stm32f4xx_hal.h"

bool interrupt_init(void){
    bool res = false;
#ifdef HAS_GPIO
    res = true;
    /* EXTI interrupt init*/
    HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);
#endif

    HAL_NVIC_SetPriority(SysTick_IRQn, 1, 0U);

#ifdef HAS_USART6
    HAL_NVIC_SetPriority(USART6_IRQn, 7, 0);
    HAL_NVIC_EnableIRQ(USART6_IRQn);
#endif

#ifdef HAS_UART7
    HAL_NVIC_SetPriority(UART7_IRQn, 7, 0);
    HAL_NVIC_EnableIRQ(UART7_IRQn);
#endif

#ifdef HAS_SPI1
    res = true;
    HAL_NVIC_SetPriority(SPI1_IRQn, 7, 0);
    HAL_NVIC_EnableIRQ(SPI1_IRQn);
#endif

#ifdef HAS_SPI2
    res = true;
    HAL_NVIC_SetPriority(SPI2_IRQn, 7, 0);
    HAL_NVIC_EnableIRQ(SPI2_IRQn);
#endif
    return res;
}
