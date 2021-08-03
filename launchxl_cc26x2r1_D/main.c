
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <ti/drivers/Board.h>
/*mandatory space NoRTOS.h needs stdint.h*/
#include <NoRTOS.h>

#include "clocks.h"
#include "common_functions.h"
#include "gpio_drv.h"
#include "hw_init.h"
#include "io_utils.h"
#include "log.h"
#include "sw_init.h"
#include "uart_drv.h"

int main(void) {
    bool res = false;
    pause_ms(2000);

    res = hw_init() && res;
    res = sw_init() && res;
    io_printf("init %s" CRLF, res ? "OF" : "Error");

    common_main_loop();
    /*Unreachable line*/
    while(1) {
    }
}
