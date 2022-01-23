#ifndef SYSTEM_CONFIG_H
#define SYSTEM_CONFIG_H

#include "t_bram_v1_1.h"
#include "sys_config_common.h"

/*Hardware config*/
#define UART_NUM_CLI (0)
#define MAIN_SPI_NUM 1

/*Software config*/
#define DIO_LED_HEARTBEAT DIO_LED_RED
#define LED_INDEX_HEARTBEAT LED_INDEX_RED
#define APP_START_ADDRESS 0x40000000
#define SPI_CLK_FREQUENCY_HZ 2000000

#endif /* SYSTEM_CONFIG_H  */
