#ifndef SYS_CONFIG_H
#define SYS_CONFIG_H

#include "board_stm32_h407.h"
#include "boot_cfg.h"

#define LED_INDEX_GREEN 0
#define UART_NUM_CLI (6U-1)

#define WDT_TIMEOUT_MS 10000
#define CLI_UART_BAUD_RATE 460800 /* bit/s */

#endif /* SYS_CONFIG_H  */
