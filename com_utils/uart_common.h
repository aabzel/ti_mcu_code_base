#ifndef UART_COMMON_H
#define UART_COMMON_H

//#include "device.h"
#include <stdbool.h>
#include <stdint.h>

#ifdef CUBEMX
typedef UART_HandleTypeDef UartHandle_t;
#else
typedef struct xUartHandle_t {
  bool tx_int;
  bool rx_int;
  uint8_t rx_byte;
  uint8_t rx_byte_cnt;
  uint8_t tx_byte_cnt;
  uint32_t rx_cnt;
  uint32_t tx_cnt;
  uint32_t tx_cpl_cnt;
  bool init_done;
  uint32_t* base_address;
}UartHandle_t;
#endif

typedef enum {
  UART_ERROR_PE,
  UART_ERROR_FE,
  UART_ERROR_NE,
  UART_ERROR_ORE
} rx_uart_error_t;

const char *uart_error2str(rx_uart_error_t e);
#endif /* UART_COMMON_H */
