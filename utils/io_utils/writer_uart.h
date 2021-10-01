#ifndef SRC_UART_WRITER_H
#define SRC_UART_WRITER_H

#include "writer_generic.h"
#include <stdbool.h>

bool uart_writer_transmit(struct generic_writer_s* s);

#endif /* SRC_UART_WRITER_H */
