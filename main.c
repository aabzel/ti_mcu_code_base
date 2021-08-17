
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <ti/drivers/Board.h>
/*mandatory space NoRTOS.h needs stdint.h*/
#include <NoRTOS.h>

#include "clocks.h"
#include "common_functions.h"
#include "debug_info.h"
#ifdef HAS_HEALTH_MONITOR
#include "health_monitor.h"
#endif /*HAS_HEALTH_MONITOR*/
#include "gpio_drv.h"
#include "hw_init.h"
#include "io_utils.h"
#include "log.h"
#include "sw_init.h"
#include "uart_drv.h"

static bool sys_init(void) {
    bool res = true;
    res = hw_init() && res;
    res = sw_init() && res;
    return res;
}

int main(void) {
    bool res = true;
    pause_ms(20);

    res = sys_init() && res;

    io_printf(CRLF "init [%s]" CRLF, (true == res) ? "OK!" : "Error!");
#ifdef HAS_HEALTH_MONITOR
    HealthMon.init_error = !res;
#endif /*HAS_HEALTH_MONITOR*/

    print_version();
    print_sys_info();

    common_main_loop();
    /*Unreachable line*/
    while(1) {
    }
}
