#ifndef SYS_CONFIG_H
#define SYS_CONFIG_H

#define APP_PAGE_START 12U
#define APP_PAGE_CNT 26U
#define APP_START_ADDRESS 0x00018000
#define CLI_UART_NUM 0U
#define WDT_TIMEOUT_MS 10000
/*DIO config */
#define DIO_3V3_CTRL 21U
#define DIO_BATT_SCL 5U
#define DIO_BATT_SDA 6U
#define DIO_GNSS_INT 20U
#define DIO_LED_GREEN 30U /*STATUS_01*/
#define DIO_LED_RED 26U
#define DIO_LEN 22U
#define DIO_PS_RS232 7U /*Enable RS232 transceiver*/
#define DIO_PWR_MUX_CTRL 14U /*PWR_MUX_CTRL*/
#define DIO_GNSS_INT 20U /*GNSS_INT*/
#define DIO_GNSS_RST_N 18U /*RST_N_GNSS*/
#define DIO_UART_CLI_RX	13U
#define DIO_UART_CLI_TX	12U

#define BOARD_NAME "Harvester"
#define MCU_NAME "CC2642R1F"
#define CLI_UART_BAUD_RATE 460800 /* bit/s */

#endif /* SYS_CONFIG_H  */