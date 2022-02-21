#ifndef SYS_CONFIG_H
#define SYS_CONFIG_H

#include "board_harvester_v1.h"
#include "boot_cfg.h"
#include "sys_config_common.h"

/*Hardware config*/
#define UART_COUNT 1
#define UART_NUM_CLI 0U
//#define UART_NUM_ZED_F9P 1U

/*Software config*/
#define APP_START_ADDRESS 0x0001A000
#define LED_INDEX_GREEN 0
#define LED_INDEX_RED 1

#endif /* SYS_CONFIG_H  */
