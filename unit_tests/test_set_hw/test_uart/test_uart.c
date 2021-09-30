#include "test_uart.h"

#include <string.h>
#include <gpio.h>

#include "uart_drv.h"
#include "sys_config.h"
#include "unit_test_check.h"
#include "uart_common.h"


bool test_uart0_write(void) {
    uint8_t array[2];
    uint32_t init_tx_cnt = huart[0].tx_cnt;
    memset(array,0xFF,sizeof(array));
    EXPECT_TRUE(uart_send(0, array, sizeof(array)));
    EXPECT_GR(init_tx_cnt, huart[0].tx_cnt,0);
    return true;
}

bool test_uart1_write(void) {
    uint8_t array[2];
    uint32_t init_tx_cnt = huart[1].tx_cnt;
    memset(array,0xFF,sizeof(array));
    EXPECT_TRUE(uart_send(1, array, sizeof(array)));
    EXPECT_GR(init_tx_cnt, huart[1].tx_cnt,0);
    return true;
}

bool test_uart0_read(void) {
    uint8_t byte=0xFF;
    EXPECT_TRUE(uart_read(0, &byte, 1));
    return true;
}

bool test_uart1_read(void) {
    uint8_t byte=0xFF;
    EXPECT_TRUE(uart_read(1, &byte, 1));
    return true;
}
