#include "writer_generic.h"

#include <string.h>

#include "uart_common.h"
#include "uart_drv.h"
#include "writer_uart.h"

static char dbg_o_data[UART_TX_ARRAY_SIZE];

generic_writer_t dbg_o = {
    .s = {writer_putc, writer_puts},
    .in_transmit = 0,
    .lost_char_count = 0,
    .total_char_count = 0,
    .error_count = 0,
    .fifo = {{sizeof(dbg_o_data), 0, 0, 0, 0}, dbg_o_data},
    .f_transmit = uart_writer_transmit,
};

void writer_putc(void *_s, char ch) {
  generic_writer_t *s = (generic_writer_t *)_s;
  if (!fifo_char_add(&s->fifo, ch)) {
    s->lost_char_count++;
  } else {
    s->f_transmit(s);
  }
}

void writer_puts(void *_s, const char *str, int32_t len) {
  if (len == 1) {
    writer_putc(_s, *str);
  } else {
    generic_writer_t *s = (generic_writer_t *)_s;

    if (len < 0) {
      len = strlen(str);
    }
    while (len) {
      if (!fifo_char_add(&s->fifo, *str)) {
        s->lost_char_count += len;
      }
      str++;
      len--;
    }
    if (fifo_char_get_used(&s->fifo)) {
      s->f_transmit(s);
    }
  }
}

void writer_error_callback(generic_writer_t *s) { s->error_count++; }

bool writer_clean(const generic_writer_t *s) {
  return ((fifo_char_get_used(&s->fifo) == 0));
}

bool writer_half_clean(const generic_writer_t *s) {
  { return (fifo_char_get_used(&s->fifo) < fifo_char_get_size(&s->fifo) / 2); }
}
