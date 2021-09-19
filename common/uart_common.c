#include "uart_common.h"
#include "convert.h"

const char *uart_error2str(rx_uart_error_t e) {
  const char *result;
  switch (e) {
  case UART_ERROR_PE:
    result = "PE";
    break;
  case UART_ERROR_FE:
    result = "FE";
    break;
  case UART_ERROR_NE:
    result = "NE";
    break;
  case UART_ERROR_ORE:
    result = "ORE";
    break;
  default:
    result = rx_utoa32(e);
    break;
  }
  return result;
}
