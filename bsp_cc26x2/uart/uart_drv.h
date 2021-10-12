#ifndef USART_DRV_H
#define USART_DRV_H

#include <hw_memmap.h>
#include <ti/drivers/UART.h>
#include <ti/drivers/uart/UARTCC26XX.h>

#include "sys_config.h"

#ifdef HAS_BOOTLOADER
#define UART_COUNT 1
#else
#define UART_COUNT 2
#endif /*HAS_BOOTLOADER*/

#define UART_FIFO_TX_SIZE 256U
#define UART_FIFO_RX_SIZE 512U

#define UART_RX_ARRAY_SIZE 500U
#define UART_RX_FIFO_ARRAY_SIZE 100U
#define UART_TX_ARRAY_SIZE 600U

#define UART0_BAUD_RATE CLI_UART_BAUD_RATE /*CLI   */
#define UART1_BAUD_RATE 38400U             /*UBlox*/

#define CONFIG_TI_DRIVERS_UART_COUNT 1

extern const uint_least8_t CONFIG_UART_0_CONST;

extern const UARTCC26XX_HWAttrsV2 uartCC26XXHWAttrs[UART_COUNT];

#ifdef HAS_UART0_FWD
bool proc_uart0_fwd(void);
#endif
#ifdef HAS_UART1_FWD
bool proc_uart1_fwd(void);
#endif
bool uart_init(void);
bool uart_deinit(uint8_t uart_num);
bool is_uart_valid(uint8_t uart_num);
bool uart_read(uint8_t uart_num, uint8_t* out_array, uint16_t array_len);
bool uart_send(uint8_t uart_num, uint8_t* array, uint16_t array_len, bool is_wait);
uint32_t uart_get_baud_rate(uint8_t uart_num, uint16_t* mantissa, uint16_t* fraction, uint8_t* over_sampling);
bool uart_set_baudrate(uint8_t uart_num, uint32_t baudrate);
uint32_t uart_get_baudrate(uint8_t uart_num);
bool proc_uart(uint8_t uart_index);
bool proc_uarts(void);

#endif /* USART_DRV_H  */
