#ifndef SRC_UART_WRITER_H
#define SRC_UART_WRITER_H

#include <stdbool.h>

#include "writer_generic.h"

extern generic_writer_t dbg_o;
#define DBG_UART_STREAM (&dbg_o.s)

bool uart_writer_transmit(struct generic_writer_s* s);

#endif /* SRC_UART_WRITER_H */
