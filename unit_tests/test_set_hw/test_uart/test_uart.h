#ifndef TEST_UART_H
#define TEST_UART_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

bool test_uart1_read(void);
bool test_uart1_write(void);
bool test_uart0_read(void);
bool test_uart0_write(void);

#define TEST_SUIT_UART                                                                                                 \
    {"uart0_write", test_uart0_write}, {"uart0_read", test_uart0_read}, {"uart1_write", test_uart1_write},             \
        {"uart1_read", test_uart1_read},

#ifdef __cplusplus
}
#endif

#endif /* TEST_UART_H */
