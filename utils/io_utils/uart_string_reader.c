#include "uart_string_reader.h"

#include <stdint.h>
#include <string.h>

#include "cli_manager.h"
#include "io_utils.h"
#include "sys.h"
#include "timer_utils.h"
#include "fifo_char.h"
#include "uart_drv.h"

static char cmd_reader_data[UART_RX_FIFO_ARRAY_SIZE];
static char cmd_reader_string[UART_RX_ARRAY_SIZE];

uart_string_reader_t cmd_reader = {
    .string_size = sizeof(cmd_reader_string),
    .callback = (handle_string_f) (process_shell_cmd),
    .string = cmd_reader_string,
    .fifo = {.fifoState={(sizeof(cmd_reader_data)), 0, 0, 0, 0},
             .array=cmd_reader_data,
             .initDone=true
     },
    .string_len = 0,
    .error_count = 0,
    .lost_char_count = 0,
    .total_char_count = 0,
    .total_string_count = 0,
};

uart_string_reader_t* get_uart_reader(void) { return &cmd_reader; }

void uart_string_reader_clear_str(uart_string_reader_t* rdr) {
    rdr->string_len = 0;
    rdr->string[0] = 0;
}

bool uart_string_reader_init(uart_string_reader_t* rdr) {
    (void)(rdr);
    //cli_tune_read_char();
    return true;
}

void uart_string_reader_rx_callback(uart_string_reader_t* rdr, char c) {
    //if (false == fifo_char_add(&rdr->fifo, c)) {
    if (false == fifo_push(&rdr->fifo, c)) {
        rdr->lost_char_count++;
    }
}

void uart_string_reader_error_callback(uart_string_reader_t* rdr) {
    rdr->error_count++;
   // cli_tune_read_char();
}

void uart_string_reader_proccess(uart_string_reader_t* rdr) {
    while (1) {
        fifo_index_t size = 0, i=0;
        char p[200];
        //const char* p = fifo_char_get_contiguous_block(&rdr->fifo, &size);
        fifo_pull_array(&rdr->fifo, p, &size);
        if (0 == size) {
            break;
        }
        for (i = 0; i < size; i++) {
            char character = p[i];
            rdr->total_char_count++;
            if (character != '\n') {
                if (cli_echo) {
                    io_putchar(character);
                }
                switch (character) {
                case '\b':
                    if (rdr->string_len) {
                        rdr->string[rdr->string_len] = 0;
                        rdr->string_len--;
                    }
                    if (cli_echo) {
                        io_putchar(' ');
                        io_putchar('\b');
                    }
                    break;
                case '\r':
                    if (cli_echo) {
                        io_putchar('\n');
                    }
                    rdr->string[rdr->string_len] = 0;
                    rdr->total_string_count++;
                    if (rdr->callback) {
                        rdr->callback(rdr->string);
                    }
                    rdr->string_len = 0;
                    rdr->string[0] = 0;
                    break;
                default:
                    if (rdr->string_len < rdr->string_size) {
                        rdr->string[rdr->string_len] = character;
                        rdr->string_len++;
                    } else {
                        rdr->lost_char_count++;
                    }
                    break;
                }
            }
        }
        //fifo_char_free(&rdr->fifo, size);
    }
}

const char* uart_string_reader_get_str(const uart_string_reader_t* rdr) { return rdr->string; }


