
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <ti/drivers/Board.h>
/*mandatory space*/
#include <NoRTOS.h>

#include "common_functions.h"
#include "gpio_drv.h"
#include "uart_drv.h"
#include "io_utils.h"
#include "log.h"
#include "sw_init.h"

static void init_hw(void) {
    Board_init();
    NoRTOS_start();
    init_gpio();
    init_uart();
}


uint32_t g_iteration_cnt = 10;

int main(void) {
    bool res=false;
    init_hw();
    sw_init();
    io_printf("init %s" CRLF, res ? "OF" : "Error");
    common_main_loop();


}
