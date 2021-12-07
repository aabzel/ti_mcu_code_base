#include "sys_tick.h"

#include <stdbool.h>
#include <stdint.h>

//#include "core_cm4.h"
#include "clocks.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_cortex.h"

volatile uint32_t g_up_time_ms = 0;

#ifdef HAS_SYS_TIC_INT
/*overflow after 49 days*/
void SysTickIntHandler(void) { g_up_time_ms++; }
#endif /*HAS_SYS_TIC_INT*/

bool SysTickInit(void) {
    bool res = true;
    g_up_time_ms = 0;
    uint32_t ret = 0;
    // 1mS interrupt timing
    uint32_t ticks = 168000000 / 1000;
    ret = HAL_SYSTICK_Config(ticks);
    if(ret) {
        res = false;
    }
#ifdef HAS_SYS_TIC_INT
    HAL_NVIC_SetPriority(SysTick_IRQn, 1, 0U);
    //  SysTickIntRegister(SysTickIntHandler);
#endif /*HAS_SYS_TIC_INT*/
    return res;
}
