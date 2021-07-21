#ifndef INIT_UART_H
#define INIT_UART_H

#include <ti/drivers/UART.h>

#define CONFIG_UART_COUNT 1
/*
 *  TX: DIO3
 *  RX: DIO2
 */

#define CONFIG_UART_0 0
#define CONFIG_TI_DRIVERS_UART_COUNT 1

extern const uint_least8_t CONFIG_UART_0_CONST;

extern UART_Handle uart;
extern char input;

void init_uart(void);
void proc_uart(void);

#endif /* INIT_UART_H  */
