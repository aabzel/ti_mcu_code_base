#ifndef WRITER_CONFIG_H
#define WRITER_CONFIG_H

#include "writer_generic.h"

#ifdef HAS_LORA
#include "writer_lora.h"
#endif /*HAS_LORA*/

#ifdef HAS_CAN
#include "writer_can.h"
#endif /*HAS_LORA*/

#ifdef HAS_UART
#include "writer_uart.h"
#endif /*HAS_UART*/

extern generic_writer_t *curWriterPtr;

bool writer_init(void);

#endif /* WRITER_CONFIG_H */
