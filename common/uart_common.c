#include "uart_common.h"

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "data_utils.h"
#ifdef HAS_UART
#include "uart_drv.h"
#endif

#ifdef HAS_UART
UartHandle_t huart[UART_COUNT] = {0};

bool uart_common_init(uint8_t uart_num) {
    memset(&huart[uart_num - 1], 0x00, sizeof(huart[uart_num - 1]));
    huart[uart_num-1].rx_rate.min = 0xFFFFFFFF;
    return true;
}
#endif

uint32_t calc_uart_transfer_time_us(uint32_t baudrate, uint32_t bytes) {
    uint32_t tx_time_us = 0;
    float bit_time = 1.0f / (float)baudrate;
    float byte_time = 0.0f;
    byte_time = (12.0) * bit_time;
    float byte_train_duration = byte_time * bytes;
    tx_time_us = (uint32_t)(byte_train_duration * 1000000.0f);
    return tx_time_us;
}

uint32_t calc_uart_transfer_time_ms(uint32_t baudrate, uint32_t bytes) {
    uint32_t tx_time_us = 0;
    float bit_time = 1.0f / (float)baudrate;
    float byte_time = 0.0f;
    byte_time = (12.0) * bit_time;
    float byte_train_duration = byte_time * bytes;
    tx_time_us = (uint32_t)(byte_train_duration * 1000.0f);
    return tx_time_us;
}
#ifdef HAS_MCU
bool uart_calc_byte_rate(void){
    bool res = false;
    uint8_t uart_num = 0;
    for(uart_num = 0; uart_num<ARRAY_SIZE(huart); uart_num++) {
        huart[uart_num].rx_rate.cur=huart[uart_num].cnt.byte_rx - huart[uart_num].cnt_prev.byte_rx;
        huart[uart_num].rx_rate.min = min32u( huart[uart_num].rx_rate.min, huart[uart_num].rx_rate.cur);
        huart[uart_num].rx_rate.max = max32u( huart[uart_num].rx_rate.max, huart[uart_num].rx_rate.cur);
        huart[uart_num].cnt_prev.byte_rx = huart[uart_num].cnt.byte_rx;

        huart[uart_num].tx_rate.cur = huart[uart_num].cnt.byte_tx - huart[uart_num].cnt_prev.byte_tx;
        huart[uart_num].tx_rate.min = min32u( huart[uart_num].tx_rate.min, huart[uart_num].tx_rate.cur);
        huart[uart_num].tx_rate.max = max32u( huart[uart_num].tx_rate.max, huart[uart_num].tx_rate.cur);
        huart[uart_num].cnt_prev.byte_tx = huart[uart_num].cnt.byte_tx;
        res = true;
    }
    return res;
}
#endif

#ifdef HAS_UART
bool uart_send_banner(uint8_t uart_num, char pattern) {
    bool res = false;
    uint8_t banner[80] = {0};
    uint32_t i = 0;
    for(i = 0; i < sizeof(banner); i++) {
        banner[i] = pattern;
    }
    banner[0] = 0x0A;
    banner[1] = 0x0D;
    banner[sizeof(banner) - 2] = 0x0A;
    banner[sizeof(banner) - 1] = 0x0D;
    res = uart_send(uart_num, banner, sizeof(banner), true);
    return res;
}
#endif
