#include "writer_uart.h"

#include <stdbool.h>

#include "fifo_char.h"
#include "sys_config.h"
#include "uart_drv.h"

#define MAX_UART_BLOCK 100U

bool uart_writer_transmit(struct generic_writer_s* s) {
    bool res = false;
    char data[200];
    res = fifo_pull_array(&s->fifo, data, (uint16_t*)&s->in_transmit);
    if(0 < s->in_transmit) {
        s->total_char_count += s->in_transmit;
        if(MAX_UART_BLOCK < s->in_transmit) {
            s->in_transmit = MAX_UART_BLOCK;
        }
        res = uart_send(CLI_UART_NUM, (uint8_t*)data, s->in_transmit, true);
    }
    return res;
}
