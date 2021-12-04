#ifndef UART_DRV_H
#define UART_DRV_H

#include "stm32f4xx_hal.h"
#include "sys_config.h"
#include "uart_common.h"

#define UART_RX_ARRAY_SIZE 500U
#define UART_RX_FIFO_ARRAY_SIZE 100U
#define UART_TX_ARRAY_SIZE 600U

#define UART6_BAUD_RATE CLI_UART_BAUD_RATE /*CLI   */

bool init_uart(uint8_t uart_num);
bool uarts_init(void);
bool uart_read(uint8_t uart_num, uint8_t* out_array, uint16_t array_len);
bool uart_send(uint8_t uart_num, uint8_t* array, uint16_t array_len, bool is_wait);
uint32_t uart_get_baud_rate(uint8_t uart_num, uint16_t* mantissa, uint16_t* fraction, uint8_t* over_sampling);
bool uart_set_baudrate(uint8_t uart_num, uint32_t baudrate);
bool proc_uart(uint8_t uart_index);

#endif /* UART_DRV_H  */
