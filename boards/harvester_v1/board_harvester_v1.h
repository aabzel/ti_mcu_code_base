#ifndef BOARD_HARVESTER_V1_H
#define BOARD_HARVESTER_V1_H

#include "cc2642R1F.h"
/*DIO config */
#define DIO_3V3_CTRL 21U
#define DIO_BATT_ADC 25U
#define DIO_BATT_SCL 5U
#define DIO_BATT_SDA 6U
#define DIO_CAN_INT 8U
#define DIO_CAN_RST 9U
#define DIO_CAN_SS  1U
#define DIO_GNSS_INT 20U /*GNSS_INT*/
#define DIO_GNSS_RST_N 18U /*RST_N_GNSS*/
#define DIO_GNSS_RXD 11U
#define DIO_GNSS_SAFEBOOT_N 19U /*SAFEBOOT_GNSS_N*/
#define DIO_GNSS_TXD 10U
#define DIO_KL30_ADC 24U
#define DIO_LED_GREEN 26U
#define DIO_LED_RED 30U/*STATUS_01*/
#define DIO_LEN 22U
#define DIO_NTC    23U
#define DIO_PG_7V0 15U
#define DIO_PS_RS232 7U /*Enable RS232 transceiver*/
#define DIO_PWR_MUX_CTRL 14U /*PWR_MUX_CTRL*/
#define DIO_SCLK 4U
#define DIO_SPI0_MISO 2U
#define DIO_SPI0_MOSI 3U
#define DIO_SPI0_SCLK 4U
#define DIO_SX1262_BUSY 29U /*LoRa_BUSY*/
#define DIO_SX1262_INT 28U /*LoRa_INT*/
#define DIO_SX1262_RST 27U /*LoRa_NRESET*/
#define DIO_SX1262_SS 0U /*SS0_LoRa*/
#define DIO_TDI    17U
#define DIO_TDO 16U
#define DIO_UART_CLI_RX	13U
#define DIO_UART_CLI_TX	12U

#define SPI_COUNT 1
#define SYS_FREQ 48000000U
#define CLOCK_FOR_US (SYS_FREQ / 1000000U)
#define MAIN_SPI_NUM 0
#define SX1262_SPI_NUM MAIN_SPI_NUM
#define CAN_SPI_NUM MAIN_SPI_NUM

#define VOL_DIV_SCALE_PWR 11.0f
#define VOL_DIV_SCALE_ACC 1.47619f
#define LED_COUNT 2

#define LED_INDEX_GREEN 0
#define LED_INDEX_RED 1

#define BOARD_NAME "Harvester"


#endif /* BOARD_HARVESTER_V1_H  */
