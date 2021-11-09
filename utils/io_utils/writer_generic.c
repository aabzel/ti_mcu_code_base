#include "writer_generic.h"

#include <stdbool.h>
#include <string.h>

#include "fifo_char.h"
//#include "uart_common.h"
//#include "uart_drv.h"
//#include "writer_uart.h"

void writer_putc(void *_s, char ch) {
  generic_writer_t *s = (generic_writer_t *)_s;
  if (false==fifo_push(&s->fifo, ch)) {
    s->lost_char_count++;
  } else {
    s->f_transmit(s);
  }
}

void writer_puts(void *_s, const char *str, int32_t len) {
  if (1 == len) {
    writer_putc(_s, *str);
  } else {
    generic_writer_t *s = (generic_writer_t *)_s;

    if (len < 0) {
      len = strlen(str);
    }
    fifo_push_array((Fifo_array_t* ) &s->fifo, (char* ) str, (fifo_index_t) len);
    fifo_index_t size= fifo_get_count(&s->fifo);
    if (0<size) {
      s->f_transmit(s);
    }
  }
}

void writer_error_callback(generic_writer_t *s) { s->error_count++; }

bool writer_clean(const generic_writer_t *s) {
    return fifo_clean((Fifo_array_t* )&s->fifo) ;
}

bool writer_half_clean(const generic_writer_t *s) {
    return (fifo_get_count((Fifo_array_t* )&s->fifo) < (fifo_get_size((Fifo_array_t* )&s->fifo) / 2));
}
