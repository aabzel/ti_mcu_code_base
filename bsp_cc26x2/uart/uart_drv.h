#ifndef INIT_UART_H
#define INIT_UART_H

#include <ti/drivers/UART.h>

#include "uart_common.h"

#define CONFIG_UART_COUNT 2
#define UART_RX_ARRAY_SIZE 500U
#define UART_RX_FIFO_ARRAY_SIZE 100U
#define UART_TX_ARRAY_SIZE 600U

#define DEBUG_UART 0x40001000

#define CONFIG_UART_0 0 /* CLI   */
#define CONFIG_UART_1 1 /* UBlox */

#define UART0_BAUD_RATE 38400U
#define UART1_BAUD_RATE 38400U

#define CONFIG_TI_DRIVERS_UART_COUNT 1

extern const uint_least8_t CONFIG_UART_0_CONST;

extern UartHandle_t huart[CONFIG_UART_COUNT];

bool uart_init(void);

void cli_tune_read_char(void);
bool uart_send(uint8_t uart_num, uint8_t* array, uint16_t array_len);
uint32_t uart_get_baud_rate(uint8_t uart_num, uint16_t* mantissa, uint16_t* fraction, uint8_t* over_sampling);
bool uart_set_baudrate(uint8_t uart_num, uint32_t baudrate);
bool proc_uart(uint8_t uart_index);
bool proc_uart1(void);

#endif /* INIT_UART_H  */
