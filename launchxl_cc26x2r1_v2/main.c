
#include <stdint.h>
#include <stddef.h>

#include <NoRTOS.h>
#include <ti/drivers/Board.h>

#include "gpio_drv.h"
#include "uart_drv.h"

extern void *mainThread(void *arg0);

int main(void){
    Board_init();

    /* Start NoRTOS */
    NoRTOS_start();
    
	init_gpio();
    init_uart();
	
    while (1) {
        UART_read(uart, &input, 1);
        UART_write(uart, &input, 1);
    }
}
