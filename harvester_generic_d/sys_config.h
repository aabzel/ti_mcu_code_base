#ifndef SYS_CONFIG_H
#define SYS_CONFIG_H

#define APP_PAGE_START 12U
#define APP_PAGE_CNT 26U
#define APP_START_ADDRESS 0x00018000
#define CLI_UART_NUM 0U
/*DIO config */
#define DIO_LED_GREEN 30U /*STATUS_01*/
#define DIO_LED_RED   26U
#define DIO_SX1262_BUSY 29U /*LoRa_BUSY*/
#define DIO_SX1262_RST 27U /*LoRa_NRESET*/
#define DIO_SX1262_SS 0U /*SS0_LoRa*/
#define DIO_SS1_CAN 1U
#define DIO_SX1262_INT 28U /*LoRa_INT*/
#define DIO_PS_RS232 7U /*Enable RS232 transceiver */
#define DIO_SPI0_MISO 2U
#define DIO_SPI0_MOSI 3U
#define DIO_SPI0_SCLK 4U
#define DIO_UART_CLI_TX	12U
#define DIO_UART_CLI_RX	13U
#define DIO_CAN_RST 9U
#define DIO_CAN_INT_N 8U
#define DIO_PWR_MUX_CTRL 14U
#define DIO_RST_N_GNSS 18U
#define DIO_SAFEBOOT_GNSS_N 19U
#define DIO_GNSS_INT 20U
#define DIO_3V3_CTRL 21U
#define DIO_LEN 22U

#define BOARD_NAME "Harvester"
#define MCU_NAME "CC2642R1F"
#define CLI_UART_BAUD_RATE 56000 /* bit/s */

#endif /* SYS_CONFIG_H  */
