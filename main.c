
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

#ifdef HAS_BOOTLOADER
#include "boot_driver.h"
#endif /*HAS_BOOTLOADER*/

#if defined(HAS_BOOTLOADER) && defined(HAS_GENERIC)
#error "Firmware is unable to be bootloader and application simultaneously"
#endif /*HAS_BOOT and HAS_GENERIC*/

int main(void) {
    bool res = true;
#ifdef HAS_START_PAUSE
    pause_ms(20);
#endif /*HAS_START_PAUSE*/

    res = try_init(sys_init(), "SYS") && res;

    LOG_INFO(SYS, "init [%s]" , (true == res) ? "OK!" : "Error!");
#ifdef HAS_HEALTH_MONITOR
    HealthMon.init_error = !res;
#endif /*HAS_HEALTH_MONITOR*/
    io_printf("Firmware launched!" CRLF);
    print_version();
    print_sys_info();

#ifdef HAS_BOOTLOADER
    res = boot_try_app();
#endif /*HAS_BOOTLOADER*/

#ifdef NORTOS
    common_main_loop();
#endif /*NORTOS*/
    /*Unreachable line*/
    while(1) {
    }
}
