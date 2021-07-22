#include "io_utils.h"

//#include "debug_uart.h"
#include "sys.h"
#include "uart_drv.h"
#include "writer_generic.h"

void io_putstr(const char *str) {
  if (huart_dbg.init_done) {
    oputs(&dbg_o.s, str);
  }
}

void io_putchar(char ch) {
  if (huart_dbg.init_done) {
    (&dbg_o.s)->f_putch(&dbg_o.s, ch);
  }
}

void io_printf(const char *format, ...) {
  if (huart_dbg.init_done) {
    va_list vlist;
    va_start(vlist, format);
    ovprintf(&dbg_o.s, format, vlist);
    va_end(vlist);
  }
}

bool is_printf_clean(void) {
  if (huart_dbg.init_done) {
    if (!writer_clean(&dbg_o)) {
      return false;
    }
  }
  return true;
}

void wait_for_printf(void) {
  if (huart_dbg.init_done) {
    while (!writer_half_clean(&dbg_o)) {
      int32_t j;
      for (j = 0; j < 100000; j++) {
      }
      // wdt_reset_all();
    }
  }
}

bool flush_printf(void) {
  bool res = true;
  if (huart_dbg.init_done) {
    uint32_t cnt = 0;
    while ((isFromInterrupt() == false) && (false == writer_clean(&dbg_o))) {
      cnt++;
      if (100000 < cnt) {
        res = false;
        break;
      }
    }
  }
  return res;
}

ostream_t *get_console_stream(void) { return DBG_STREAM; }

void io_putstrln(const char *str) {
  io_putstr(str);
  io_putstr(CRLF);
}
