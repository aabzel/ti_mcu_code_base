#include "writer_uart.h"

#include <stdbool.h>

#include "fifo_char.h"
#include "sys_config.h"
#include "uart_drv.h"

#define MAX_UART_BLOCK 100U

bool uart_writer_transmit(struct generic_writer_s* writer) {
    bool res = false;
    char data[300] = ""; /*TODO Determine min size*/
    res = fifo_pull_array(&writer->fifo, data, (uint16_t*)&writer->in_transmit);
    if(0 < writer->in_transmit) {
        writer->total_char_count += writer->in_transmit;
        if(MAX_UART_BLOCK < writer->in_transmit) {
            writer->in_transmit = MAX_UART_BLOCK;
        }
        res = uart_send(UART_NUM_CLI, (uint8_t*)data, writer->in_transmit, true);
    }
    return res;
}
