#ifndef RS232_DRV_H
#define RS232_DRV_H

#include <stdbool.h>
#include <stdint.h>

#include "clocks.h"
#include "sys_config.h"

#define UART_NUM_RS232 UART_NUM_CLI
#define RS232_PERIOD_US S_2_US(5)

bool rs232_init(void);
bool rs232_send(uint8_t* const data, uint32_t len);
bool proc_rs232(void);

#endif /* RS232_DRV_H  */
