#ifndef SYS_CONFIG_H
#define SYS_CONFIG_H

#define APP_PAGE_START 12
#define APP_PAGE_CNT 26
#define CLI_UART_NUM 0
/*DIO config */
#define DIO_LED_GREEN 30
#define DIO_LED_RED   26
#define DIO_SX1262_BUSY 42 /*LoRa_BUSY*/
#define DIO_SX1262_RST 27 /*LoRa_NRESET*/
#define DIO_SX1262_SS 0 /*SS0_LoRa*/
#define DIO_SX1262_INT 28 /*LoRa_INT*/

#define DIO_UART_CLI_TX	12
#define DIO_UART_CLI_RX	13


#define BOARD_NAME "Harvester"
#define MCU_NAME "CC2642R1F"
#define CLI_UART_BAUD_RATE 56000 /* bit/s */

#endif /* SYS_CONFIG_H  */
