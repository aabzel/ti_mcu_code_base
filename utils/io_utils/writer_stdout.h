#ifndef SRC_STDOUT_WRITER_H
#define SRC_STDOUT_WRITER_H

#include <stdbool.h>

#include "writer_generic.h"

#define STDOUT_TX_ARRAY_SIZE 600U

extern generic_writer_t std_out_o;
#define DBG_STDOUT_STREAM (&std_out_o.s)

bool stdout_writer_transmit(struct generic_writer_s* stream);

#endif /* SRC_STDOUT_WRITER_H */
