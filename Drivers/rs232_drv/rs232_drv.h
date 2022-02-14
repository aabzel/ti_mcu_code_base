#ifndef RS232_DRV_H
#define RS232_DRV_H

#include <stdbool.h>
#include <stdint.h>

#include "clocks.h"
#include "sys_config.h"

#ifdef HAS_CLI
#define UART_NUM_RS232 UART_NUM_CLI
#else
#define UART_NUM_RS232 0
#endif
#define RS232_PERIOD_US SEC_2_US(5)

typedef struct xRS232_t {
    bool ping;
    bool link;
}RS232_t;

extern RS232_t rs232;

bool rs232_init(void);
bool rs232_send(uint8_t* const data, uint32_t len);
bool proc_rs232(void);

#endif /* RS232_DRV_H  */
