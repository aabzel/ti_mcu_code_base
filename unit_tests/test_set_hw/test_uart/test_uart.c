#include "test_uart.h"

#include <string.h>
#include <gpio.h>

#include "uart_drv.h"
#include "sys_config.h"
#include "unit_test_check.h"
#include "uart_common.h"

#ifdef HAS_UART0

bool test_uart0_write(void) {
    uint8_t array[2]={0};
    uint32_t init_tx_cnt = huart[0].cnt.byte_tx;
    memset(array,0xFF,sizeof(array));
    EXPECT_TRUE(uart_send(0, array, sizeof(array),true));
    EXPECT_GR(init_tx_cnt, huart[0].cnt.byte_tx);
    return true;
}

bool test_uart0_write_string(void) {
    char temp_str[200] = "";
    uint32_t i=0;
    memset(temp_str, 0x0, sizeof(temp_str));
    for(i=0;i<96;i++){
        temp_str[i] = '*';
    }

    io_printf("%1u", 2);
    io_printf("%s"CRLF, temp_str);

    return true;
}

bool test_uart0_read(void) {
    uint8_t byte=0xFF;
    EXPECT_FALSE(uart_read(0, &byte, 1));
    return true;
}
#endif /*HAS_UART0*/

#ifdef HAS_UART1
bool test_uart1_write(void) {
    uint8_t array[2] = {0};
    uint32_t init_tx_cnt = huart[1].cnt.byte_tx;
    memset(array,0xFF,sizeof(array));
    EXPECT_TRUE(uart_send(1, array, sizeof(array),true));
    EXPECT_GR(init_tx_cnt, huart[1].cnt.byte_tx);
    return true;
}
bool test_uart1_read(void) {
    uint8_t byte=0xFF;
    EXPECT_FALSE(uart_read(1, &byte, 1));
    return true;
}
#endif


