#include "board_layout.h"

#include <stdlib.h>
#include <ti/drivers/gpio/GPIOCC26XX.h>

#include "gpio_drv.h"

const Pin_t PinTable[DIO_CNT] = {
 {0,  5,  18,"RX",      NULL, GPIOCC26XX_DIO_02|GPIO_CFG_IN_NOPULL, PIN_INPUT_EN},
 {1,  6,  17,"TX",      NULL, GPIOCC26XX_DIO_01,PIN_PUSHPULL},
 {2,  7,  16,"RX",      NULL, GPIOCC26XX_DIO_02|GPIO_CFG_IN_NOPULL, PIN_INPUT_EN},
 {3,  8,  15,"TX",      NULL, GPIOCC26XX_DIO_03, PIN_PUSHPULL},
 {4,  9,  14,"SCL",     NULL, GPIOCC26XX_DIO_04, PIN_OPENDRAIN},
 {5,  10, 13,"SDA",     NULL, GPIOCC26XX_DIO_05, PIN_OPENDRAIN},
 {6,  11, 12,"R_LED",   NULL, GPIOCC26XX_DIO_06 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW, 6 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MED},
 {7,  12, 11,"G_LED",   NULL, GPIOCC26XX_DIO_07 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW, 7 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MED},
 {8,  14, 10,"MISO",    NULL, GPIOCC26XX_DIO_08, PIN_INPUT_EN},
 {9,  15, 9, "MOSI",    NULL, GPIOCC26XX_DIO_09, PIN_PUSHPULL},
 {10, 16, 8, "SCLK",    NULL, GPIOCC26XX_DIO_10, PIN_PUSHPULL},
 {11, 17, 7, "LoRaCS",  NULL, GPIOCC26XX_DIO_11 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_HIGH,PIN_PUSHPULL},
 {12, 18, 6, "NC",      NULL, GPIOCC26XX_DIO_12, PIN_INPUT_EN},
 {13, 19, 5, "BTN1",    NULL, GPIOCC26XX_DIO_13 | GPIO_DO_NOT_CONFIG,13 | PIN_INPUT_EN | PIN_PULLUP | PIN_IRQ_DIS},
 {14, 20, 4, "BTN2",    NULL, GPIOCC26XX_DIO_14 | GPIO_DO_NOT_CONFIG,14 | PIN_INPUT_EN | PIN_PULLUP | PIN_IRQ_DIS},
 {15, 21, 3, "NC",      NULL, GPIOCC26XX_DIO_15, PIN_INPUT_EN},
 {16, 26, 2, "TDO",     NULL, GPIOCC26XX_DIO_16, PIN_INPUT_EN},
 {17, 27, 1, "TDI",     NULL, GPIOCC26XX_DIO_17, PIN_INPUT_EN},
 {18, 28, 31,"SWO",     NULL, GPIOCC26XX_DIO_18, PIN_INPUT_EN},
 {19, 29, 30,"CTS",     NULL, GPIOCC26XX_DIO_19, PIN_INPUT_EN},
 {20, 30, 29,"FlashCS", NULL, GPIOCC26XX_DIO_20|GPIO_CFG_OUT_STD|GPIO_CFG_OUT_STR_MED|GPIO_CFG_OUT_HIGH,PIN_PUSHPULL},
 {21, 31, 28,"NC",      NULL, GPIOCC26XX_DIO_21| GPIO_CFG_IN_NOPULL, PIN_INPUT_EN},
 {22, 32, 27,"NC",      NULL, GPIOCC26XX_DIO_22| GPIO_CFG_IN_NOPULL, PIN_INPUT_EN},
 {23, 36, 26,"An",      NULL, GPIOCC26XX_DIO_23| GPIO_CFG_IN_NOPULL, PIN_INPUT_EN},
 {24, 37, 25,"An",      NULL, GPIOCC26XX_DIO_24| GPIO_CFG_IN_NOPULL, PIN_INPUT_EN},
 {25, 38, 24,"An",      NULL, GPIOCC26XX_DIO_25| GPIO_CFG_IN_NOPULL, PIN_INPUT_EN},
 {26, 39, 23,"An",      NULL, GPIOCC26XX_DIO_26| GPIO_CFG_IN_NOPULL, PIN_INPUT_EN},
 {27, 40, 22,"LoRaRst",  NULL, GPIOCC26XX_DIO_27 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_HIGH,PIN_PUSHPULL},
 {28, 41, 21,"LoRaInt",  NULL, GPIOCC26XX_DIO_29 | GPIO_CFG_IN_NOPULL, PIN_INPUT_EN},
 {29, 42, 20,"LoRaBusy", NULL, GPIOCC26XX_DIO_30 | GPIO_CFG_IN_NOPULL, PIN_INPUT_EN},
 {30, 43, 19,"An",      NULL, GPIOCC26XX_DIO_31 | GPIO_CFG_IN_NOPULL, PIN_INPUT_EN}
};



