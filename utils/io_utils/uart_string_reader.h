#ifndef SRC_UART_STRING_READER_H
#define SRC_UART_STRING_READER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

//#include "fifo_char_buffer.h"
#include "fifo_char.h"
//#include "uart_common.h"

typedef void (*handle_string_f)(char* s);

typedef struct {
    uint16_t string_size;
    handle_string_f callback;
    char* string;
    Fifo_array_t fifo;
    uint16_t string_len;
    int64_t error_count;
    int64_t lost_char_count;
    int64_t total_char_count;
    int64_t total_string_count;
} uart_string_reader_t;

extern uart_string_reader_t cmd_reader;

uart_string_reader_t* get_uart_reader(void);
bool uart_string_reader_init(uart_string_reader_t* r);
void uart_string_reader_rx_callback(uart_string_reader_t* r, char c);
void uart_string_reader_error_callback(uart_string_reader_t* r);
void uart_string_reader_proccess(uart_string_reader_t* r);
void uart_string_reader_clear_str(uart_string_reader_t* r); /* clear current string */
const char* uart_string_reader_get_str(const uart_string_reader_t* r);



#ifdef __cplusplus
}
#endif

#endif /* SRC_UART_STRING_READER_H */
