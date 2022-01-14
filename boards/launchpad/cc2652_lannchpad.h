#ifndef BOARD_LAUNCHPAD_H
#define BOARD_LAUNCHPAD_H

#include "cc2652R1F.h"

#define BOARD_NAME "LaunchPad"

#define SYS_FREQ 48000000U
#define CLOCK_FOR_US (SYS_FREQ / 1000000U)
#define DIO_LED_GREEN 7
#define DIO_LED_RED 6
#define DIO_UART_CLI_RX 2U
#define DIO_UART_CLI_TX 3U
#define DIO_BUTTON1 13
#define DIO_BUTTON2 14 

#endif /* BOARD_LAUNCHPAD_H  */
