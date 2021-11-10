#include "writer_uart.h"

#include <stdbool.h>

#include "fifo_char.h"
#include "sys_config.h"
#include "uart_drv.h"

#define MAX_UART_BLOCK 100U

static char dbg_o_data[UART_TX_ARRAY_SIZE];
generic_writer_t dbg_o = {
    .s = {writer_putc, writer_puts},
    .in_transmit = 0,
    .lost_char_count = 0,
    .total_char_count = 0,
    .error_count = 0,
    .fifo = {.fifoState = {.size = sizeof(dbg_o_data), .start = 0, .end = 0, .count = 0, .errors = 0},
             .array = dbg_o_data,
             .initDone = true},
    .f_transmit = uart_writer_transmit,
};

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
