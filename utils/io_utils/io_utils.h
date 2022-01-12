#ifndef IO_UTILS_H
#define IO_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>

#define CRLF "\r\n"

#if defined(X86_64) || defined(ESP32)
#include <stdio.h>

#define io_vprintf vprintf
#define io_printf printf
#define io_putstr puts
#define io_putchar putchar
#endif

#ifdef HAS_CUSTOM_PRINTF
void io_vprintf(const char* format, va_list vlist);
void wait_for_printf(void);
bool flush_printf(void);
void io_printf(const char* format, ...);
void io_putchar(char ch);
bool is_printf_clean(void);
void io_putstrln(const char* str);
void io_putstr(const char* str);
typedef void (*print_callback_t)(const char* str);
#define io_stdout_stream_get get_console_stream
#endif

#ifdef __cplusplus
}
#endif

#endif /* IO_UTILS_H */
