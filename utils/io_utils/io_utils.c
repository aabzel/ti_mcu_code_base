#include "io_utils.h"

#include <stdio.h>

#ifdef HAS_MCU
#include "sys.h"
#include "uart_common.h"
#include "uart_drv.h"
#include "writer_config.h"
#include "writer_generic.h"
#endif

#ifdef HAS_MCU
print_callback_t print_callback_f;
void io_putstr(const char* str) {
    if(huart[UART_NUM_CLI].init_done) {
        oputs(&curWriterPtr->s, str);
    }
}
#endif

#ifdef HAS_MCU
void io_putchar(char ch) {
    if(huart[UART_NUM_CLI].init_done) {
        (&curWriterPtr->s)->f_putch(&curWriterPtr->s, ch);
    }
}
#endif

#ifdef HAS_MCU
void io_printf(const char* format, ...) {
    if(huart[UART_NUM_CLI].init_done) {
        va_list vlist;
        va_start(vlist, format);
        ovprintf(&curWriterPtr->s, format, vlist);
        va_end(vlist);
    }
}
#endif

#ifdef HAS_MCU
void io_vprintf(const char* format, va_list vlist) { ovprintf(&curWriterPtr->s, format, vlist); }

bool is_printf_clean(void) {
    if(huart[UART_NUM_CLI].init_done) {
        if(!writer_clean(curWriterPtr)) {
            return false;
        }
    }
    return true;
}
#endif

#ifdef HAS_MCU
void io_putstrln(const char* str) {
    io_putstr(str);
    io_putstr(CRLF);
}

void wait_for_printf(void) {
    if(huart[UART_NUM_CLI].init_done) {
        while(!writer_half_clean(curWriterPtr)) {
        }
    }
}

bool flush_printf(void) {
    bool res = true;
    if(huart[UART_NUM_CLI].init_done) {
        uint32_t cnt = 0;
        while((false == isFromInterrupt()) && (false == writer_clean(curWriterPtr))) {
            cnt++;
            if(100000 < cnt) {
                res = false;
                break;
            }
        }
    }
    return res;
}
#endif

// static  ostream_t *get_console_stream(void) { return &curWriterPtr->s; }
