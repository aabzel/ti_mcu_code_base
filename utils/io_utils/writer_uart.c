#include "writer_uart.h"

#include "fifo_char_buffer.h"
#include "uart_drv.h"
#include "sys_config.h"

#define MAX_UART_BLOCK 100U

void uart_writer_transmit(struct generic_writer_s *s) {
  const char *data = NULL;
  if (s->in_transmit) {
    fifo_char_free(&s->fifo, s->in_transmit);
  }
  data =
      fifo_char_get_contiguous_block(&s->fifo, (fifo_index_t *)&s->in_transmit);
  if (0 < s->in_transmit) {
    s->total_char_count += s->in_transmit;
    if (MAX_UART_BLOCK < s->in_transmit) {
      s->in_transmit = MAX_UART_BLOCK;
    }
    uart_send(CLI_UART_NUM,(uint8_t *)data, s->in_transmit);
  }
}
