#ifndef UART_DRV_H
#define UART_DRV_H

#include "stm32f4xx_hal.h"
#include "sys_config.h"
#include "uart_common.h"

#define UART_RX_ARRAY_SIZE 500U
#define UART_RX_FIFO_ARRAY_SIZE 100U
#define UART_TX_ARRAY_SIZE 600U

#define UART_COUNT 10
#define UART7_BAUD_RATE CLI_UART_BAUD_RATE /*CLI   */

extern UartHandle_t huart[UART_COUNT];

bool uart_init(void);
void cli_tune_read_char(void);
bool uart_read(uint8_t uart_num, uint8_t* out_array, uint16_t array_len);
bool uart_send(uint8_t uart_num, uint8_t* array, uint16_t array_len);
uint32_t uart_get_baud_rate(uint8_t uart_num, uint16_t* mantissa, uint16_t* fraction, uint8_t* over_sampling);
bool uart_set_baudrate(uint8_t uart_num, uint32_t baudrate);
bool proc_uart(uint8_t uart_index);

#endif /* UART_DRV_H  */
