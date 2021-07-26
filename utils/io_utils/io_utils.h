#ifndef IO_UTILS_H
#define IO_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#define CRLF "\r\n"

#define io_puts io_putstr
#define io_putc io_putchar
#define io_flush flush_printf

#define io_stdout_stream_get get_console_stream

void io_putchar(char ch);
void io_putstr(const char *str);
bool flush_printf(void);
void wait_for_printf(void);
bool is_printf_clean(void);
void io_putstrln(const char *str);
void io_printf(const char *format, ...);

#ifdef __cplusplus
}
#endif

#endif /* IO_UTILS_H */
