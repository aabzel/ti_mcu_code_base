#include "io_utils.h"

#include <stdio.h>

#ifdef HAS_MCU
#include "sys.h"
#endif

#ifdef HAS_LOG
#include "writer_config.h"
#include "writer_generic.h"
#endif

#ifdef HAS_UART
#include "uart_common.h"
#include "uart_drv.h"
#endif


#ifdef HAS_CUSTOM_PRINTF
print_callback_t print_callback_f;
void io_putstr(const char* str) {
    if(huart[UART_NUM_CLI].init_done) {
#ifdef HAS_LOG
        oputs(&curWriterPtr->s, str);
#endif
    }
}
#endif

#ifdef HAS_CUSTOM_PRINTF
void io_putchar(char ch) {
    if(huart[UART_NUM_CLI].init_done) {
#ifdef HAS_LOG
        (&curWriterPtr->s)->f_putch(&curWriterPtr->s, ch);
#endif
    }
}
#endif

#ifdef HAS_CUSTOM_PRINTF
void io_printf(const char* format, ...) {
    if(huart[UART_NUM_CLI].init_done) {
        va_list vlist;
        va_start(vlist, format);
#ifdef HAS_LOG
        ovprintf(&curWriterPtr->s, format, vlist);
#endif
        va_end(vlist);
    }
}
#endif

#ifdef HAS_CUSTOM_PRINTF
void io_vprintf(const char* format, va_list vlist) {
#ifdef HAS_LOG
    ovprintf(&curWriterPtr->s, format, vlist);
#endif
}

bool is_printf_clean(void) {
    bool res = false;
    if(huart[UART_NUM_CLI].init_done) {
#ifdef HAS_LOG
        res = writer_clean(curWriterPtr);
#endif
    }
    return res;
}
#endif

#ifdef HAS_CUSTOM_PRINTF
void io_putstrln(const char* str) {
#ifdef HAS_LOG
    io_putstr(str);
    io_putstr(CRLF);
#endif
}

void wait_for_printf(void) {
#ifdef HAS_LOG
    if(huart[UART_NUM_CLI].init_done) {
        while(!writer_half_clean(curWriterPtr)) {
        }
    }
#endif
}

bool flush_printf(void) {
    bool res = false;
    if(huart[UART_NUM_CLI].init_done) {
       res = true;
#ifdef HAS_LOG
        uint32_t cnt = 0;
        while((false == isFromInterrupt()) && (false == writer_clean(curWriterPtr))) {
            cnt++;
            if(100000 < cnt) {
                res = false;
                break;
            }
        }
#endif
    }
    return res;
}
#endif

