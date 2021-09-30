#include "uart_common.h"

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

UartHandle_t huart[UART_COUNT] = {0};


bool uart_common_init(uint8_t uart_num){
    memset(&huart[uart_num], 0x00, sizeof(huart[uart_num]));
    return true;
}
