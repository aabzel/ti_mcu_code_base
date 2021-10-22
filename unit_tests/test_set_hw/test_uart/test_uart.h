#ifndef TEST_UART_H
#define TEST_UART_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#ifdef HAS_UART0
bool test_uart0_read(void);
bool test_uart0_write(void);

#define TEST_SUIT_UART0                 \
    {"uart0_write", test_uart0_write},  \
    {"uart0_read", test_uart0_read},
#else
#define TEST_SUIT_UART0
#endif

#ifdef HAS_UART1
bool test_uart1_read(void);
bool test_uart1_write(void);

#define TEST_SUIT_UART1                     \
    {"uart1_write", test_uart1_write},      \
    {"uart1_read", test_uart1_read},
#else
#define TEST_SUIT_UART1
#endif

#define TEST_SUIT_UART \
    TEST_SUIT_UART0 \
    TEST_SUIT_UART1

#ifdef __cplusplus
}
#endif

#endif /* TEST_UART_H */
