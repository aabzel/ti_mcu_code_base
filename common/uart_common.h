#ifndef UART_COMMON_H
#define UART_COMMON_H

#include <stdbool.h>
#include <stdint.h>

#ifdef DeviceFamily_CC26X2
#include <ti/devices/cc13x2_cc26x2/inc/hw_memmap.h>
#include <ti/devices/cc13x2_cc26x2/inc/hw_ints.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/Power.h>
#include <ti/drivers/UART.h>
#include <ti/drivers/power/PowerCC26X2.h>
#include <ti/drivers/uart/UARTCC26XX.h>
#endif

#ifdef USE_HAL_DRIVER
#include "stm32f4xx_hal.h"
#endif /*USE_HAL_DRIVER*/

typedef struct xUartHandle_t {
  volatile bool tx_int;
  volatile bool rx_int;
  bool rx_it_proc_done;
  volatile uint8_t rx_byte;
  volatile uint8_t rx_byte_it;
  uint8_t *rx_buff;
  uint8_t rx_byte_cnt;
  uint8_t tx_byte_cnt;
  volatile uint32_t rx_cnt;
  volatile uint32_t tx_cnt;
  uint32_t rx_buff_size;
  uint32_t error_cnt;
  volatile uint32_t tx_cpl_cnt;
  bool init_done;
  uint32_t* base_address;
#ifdef DeviceFamily_CC26X2
  UART_Handle uart_h;
  UART_Params uartParams;
#endif
#ifdef  USE_HAL_DRIVER 
  UART_HandleTypeDef uart_h;
#endif /*USE_HAL_DRIVER*/
  char name[8];
}UartHandle_t;


typedef enum {
  UART_ERROR_PE,
  UART_ERROR_FE,
  UART_ERROR_NE,
  UART_ERROR_ORE
} rx_uart_error_t;

const char *uart_error2str(rx_uart_error_t e);
#endif /* UART_COMMON_H */
