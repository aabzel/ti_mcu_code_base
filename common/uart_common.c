#include "uart_common.h"

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#ifdef HAS_MCU
UartHandle_t huart[UART_COUNT] = {0};

bool uart_common_init(uint8_t uart_num){
    memset(&huart[uart_num-1], 0x00, sizeof(huart[uart_num-1]));
    return true;
}
#endif

uint32_t calc_uart_transfer_time_us(uint32_t baudrate, uint32_t bytes) {
    uint32_t tx_time_us = 0;
    float bit_time = 1.0f / (float) baudrate;
    float byte_time = 0.0f;
    byte_time = (12.0)*bit_time;
    float byte_train_duration = byte_time*bytes;
    tx_time_us = (uint32_t) (byte_train_duration*1000000.0f);
    return tx_time_us;
}

uint32_t calc_uart_transfer_time_ms(uint32_t baudrate, uint32_t bytes) {
    uint32_t tx_time_us = 0;
    float bit_time = 1.0f / (float) baudrate;
    float byte_time = 0.0f;
    byte_time = (12.0)*bit_time;
    float byte_train_duration = byte_time*bytes;
    tx_time_us = (uint32_t) (byte_train_duration*1000.0f);
    return tx_time_us;
}
