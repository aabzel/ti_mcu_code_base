
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <ti/drivers/Board.h>

#ifdef NORTOS
/*mandatory space NoRTOS.h needs stdint.h*/
#include <NoRTOS.h>
#endif /*NORTOS*/

#include "clocks.h"
#include "common_functions.h"
#include "debug_info.h"

#ifdef HAS_HEALTH_MONITOR
#include "health_monitor.h"
#endif /*HAS_HEALTH_MONITOR*/

#include "gpio_drv.h"
#include "io_utils.h"
#include "log.h"
#include "uart_drv.h"

#if defined(HAS_BOOT) && defined(HAS_GENERIC)
#error "Firmware is unable to be bootloader and application simultaneously"
#endif /*HAS_BOOT and HAS_GENERIC*/

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

#ifdef NORTOS
    common_main_loop();
#endif /*NORTOS*/
    /*Unreachable line*/
    while(1) {
    }
}
