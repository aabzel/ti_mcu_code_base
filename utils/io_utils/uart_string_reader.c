#include "uart_string_reader.h"

#include <stdint.h>
#include <string.h>

#include "cli_manager.h"
#include "clocks.h"
#include "fifo_char.h"
#include "io_utils.h"
#include "sys.h"
#include "timer_utils.h"
#include "uart_drv.h"

static char cmd_reader_data[UART_RX_FIFO_ARRAY_SIZE];
static char cmd_reader_string[UART_RX_ARRAY_SIZE];

uart_string_reader_t cmd_reader = {
    .string_size = sizeof(cmd_reader_string),
    .callback = (handle_string_f)(process_shell_cmd),
    .string = cmd_reader_string,
    .fifo = {.fifoState = {(sizeof(cmd_reader_data)), 0, 0, 0, 0}, .array = cmd_reader_data, .initDone = true},
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
    return true;
}

bool uart_string_reader_rx_callback(uart_string_reader_t* rdr, char c) {
    bool res = false;
    if(rdr) {
        if(false == fifo_push(&rdr->fifo, c)) {
            rdr->lost_char_count++;
        } else {
            res = true;
        }
    }
    return res;
}

void uart_string_reader_error_callback(uart_string_reader_t* rdr) { rdr->error_count++; }

#define CLI_POLL_TIMEOUT_MS 500
bool uart_string_reader_proccess(uart_string_reader_t* rdr) {
    bool res = true;
#ifdef HAS_CLOCK
    uint32_t cur_time_ms = 0;
    uint32_t duration_ms = 0;
    uint32_t start_time_ms = get_time_ms32();
#endif /*HAS_CLOCK*/
    while(1) {
#ifdef HAS_CLOCK
        cur_time_ms = get_time_ms32();
        duration_ms = cur_time_ms - start_time_ms;
        if(CLI_POLL_TIMEOUT_MS < duration_ms) {
            res = false;
            break;
        }
#endif /*HAS_CLOCK*/
        fifo_index_t size = 0, i = 0;
        char data[200] = "";
        fifo_pull_array(&rdr->fifo, data, sizeof(data), &size);
        if(0 == size) {
            break;
        }

#ifdef HAS_CLI_CMD_HISTORY
        Arrow_t arr = ARROW_UNDEF;
#endif
        for(i = 0; i < size; i++) {
            char character = data[i];
            rdr->total_char_count++;
#ifdef HAS_CLI_CMD_HISTORY
            arr = cli_arrows_parse(character);
            if(ARROW_UP == arr) {
                io_printf("_%s", prev_cmd);
                strncpy(rdr->string, prev_cmd, rdr->string_size);
                rdr->string_len = strlen(prev_cmd);
                character = 0x00;
            }
#endif
            if(character != '\n') {
                if(cli_echo) {
                    io_putchar(character);
#ifdef ESP32
                    fflush(stdout);
#endif
                }
                switch(character) {
                case '\b':
                    if(rdr->string_len) {
                        rdr->string[rdr->string_len] = 0;
                        rdr->string_len--;
                    }
                    if(cli_echo) {
                        io_putchar(' ');
                        io_putchar('\b');
                    }
                    break;
                case 0x1B:
                    break;
                case 0x0D: //'\r'://
                    if(cli_echo) {
                        io_putchar('\n');
                    }
                    rdr->string[rdr->string_len] = 0;
                    rdr->total_string_count++;
                    if(rdr->callback) {
                        rdr->callback(rdr->string);
                    }
                    rdr->string_len = 0;
                    rdr->string[0] = 0;
                    break;
                default:
                    if(rdr->string_len < rdr->string_size) {
                        rdr->string[rdr->string_len] = character;
                        rdr->string_len++;
                    } else {
                        rdr->lost_char_count++;
                    }
                    break;
                }
            }
        }
    }
    return res;
}

const char* uart_string_reader_get_str(const uart_string_reader_t* rdr) { return rdr->string; }
