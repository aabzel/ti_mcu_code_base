/**
 *  Description : manager for Bash-like UART console.
 *  Command Line Interface (CLI)
 */

#include "cli_manager.h"
#include "uart_drv.h"
#include "shell.h"
#include "uart_string_reader.h"
#include "writer_generic.h"
#include "uart_drv.h"

uint32_t cli_task_cnt = 0;
bool cli_init_done = false;

void cli_init(void) {
    if (false == uart_string_reader_init(&cmd_reader)) {
        cli_init_done = false;
    } else {
        set_echo(true);
        cli_init_done = true;
    }
}

bool cli_process(void) {
    if (true==huart_dbg.rx_int) {
        uart_string_reader_rx_callback(&cmd_reader, (char) huart_dbg.rx_byte );
        huart_dbg.rx_int = false;
        UART_read(uart_0, &huart_dbg.rx_byte, 1);
    }

    bool res = false;
    static bool entry = false;
    if (false == entry) {
        /* recursive protection from test which call during execution */
        if (true == cli_init_done) {
            entry = true;
            cli_task_cnt++;
            if (true == huart_dbg.tx_int) {
                dbg_o.f_transmit(&dbg_o);
                huart_dbg.tx_int = false;
            }
            uart_string_reader_proccess(&cmd_reader);
            res = true;
            entry = false;
        }
    }
    return res;
}
