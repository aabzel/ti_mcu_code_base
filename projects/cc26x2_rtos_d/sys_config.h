#ifndef SYS_CONFIG_H
#define SYS_CONFIG_H

#include "cc2652_lannchpad.h"

/*hardware config*/
#define UART_COUNT 1
#define LED_COUNT 2

/*Software config*/
#define DFLT_TIME_ZONE 3
#define APP_START_ADDRESS 0x00000000
#define CLI_UART_BAUD_RATE 460800U /* bit/s */
#define LED_INDEX_GREEN 0
#define LED_INDEX_RED 1
#define UART_NUM_CLI 0U
#define WDT_TIMEOUT_MS 10000U

#endif /* SYS_CONFIG_H  */
