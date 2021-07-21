
#include <stddef.h>
#include <stdint.h>
#include <ti/drivers/Board.h>

#include <NoRTOS.h>

#include "gpio_drv.h"
#include "uart_drv.h"

static void init_hw(void) {
    Board_init();
    NoRTOS_start();
    init_gpio();
    init_uart();
}

static void common_loop(void) { proc_uart(); }

int main(void) {

    init_hw();

    while(1) {
        common_loop();
    }
}
