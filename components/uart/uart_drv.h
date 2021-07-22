#ifndef INIT_UART_H
#define INIT_UART_H

#include <ti/drivers/UART.h>

#include "uart_common.h"

#define CONFIG_UART_COUNT 1
/*
 *  TX: DIO3
 *  RX: DIO2
 */
#define UART_RX_ARRAY_SIZE 500U
#define UART_RX_FIFO_ARRAY_SIZE 100U
#define UART_TX_ARRAY_SIZE 600U
#define DEBUG_UART 0x40001000
#define CONFIG_UART_0 0
#define CONFIG_TI_DRIVERS_UART_COUNT 1

extern const uint_least8_t CONFIG_UART_0_CONST;
extern UART_Handle uart_0;
extern char input;
extern UartHandle_t huart_dbg;

void init_uart(void);
void proc_uart(void);
void tune_read_char(void);
bool uart_send_debug(const uint8_t *tx_buffer, uint16_t len) ;

#endif /* INIT_UART_H  */
