#include "board_layout.h"

#include <stdlib.h>
#include <ti/drivers/gpio/GPIOCC26XX.h>

#include "gpio_drv.h"
#include "sys_config.h"

/*line order does not  matters!*/
const Pin_t PinTable[DIO_CNT] = {
 {DIO_PWR_MUX_CTRL,   20,  0,   "PwrMux", NULL, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW, PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MED},
 {DIO_LED_RED,        39,  0,    "R_LED", NULL, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW, PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MED},
 {DIO_LED_GREEN,      43,  0,    "G_LED", NULL, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW, PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MED},
 {GPIOCC26XX_DIO_12,  18,  0,   "MCU_TX", NULL, GPIO_CFG_OUTPUT, PIN_PUSHPULL},
 {GPIOCC26XX_DIO_13,  19,  0,   "MCU_RX", NULL, GPIO_CFG_IN_NOPULL, PIN_INPUT_EN},
 {DIO_PS_RS232,       12,  0, "PS_RS232", NULL, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW, PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MED},
 {GPIOCC26XX_DIO_16,  26,  0,      "TDO", NULL, GPIO_CFG_OUTPUT, PIN_INPUT_EN},
 {GPIOCC26XX_DIO_17,  27,  0,      "TDI", NULL, GPIO_CFG_INPUT,  PIN_INPUT_EN},
 {DIO_GNSS_RST_N,     28,  0, "GNSS_Rst", NULL, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW,   PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MED},
};
/*line order matters!*/
GPIO_PinConfig gpioPinConfigs[GPIO_COUNT] = {
    DIO_LED_GREEN | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW, /* LED Green */
    DIO_LED_RED | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW, /* LED Red */
    DIO_PS_RS232 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_HIGH,  /*RS232 TX enable*/
    DIO_PWR_MUX_CTRL | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW, /* PWR_MUX_CTRL */
    DIO_GNSS_RST_N | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW, /* RST_N_GNSS */
};

/*line order matters!*/
PIN_Config BoardGpioInitTable[GPIO_COUNT + 1] = {
    DIO_LED_GREEN | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MED,/* LED Green */
    DIO_LED_RED | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MED,/* LED Red */
    DIO_PS_RS232 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_HIGH | PIN_PUSHPULL | PIN_DRVSTR_MED,/*RS232 TX enable*/
    DIO_PWR_MUX_CTRL | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MED,/* PWR_MUX_CTRL */
    DIO_GNSS_RST_N | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MED,/* RST_N_GNSS */
    PIN_TERMINATE
};

GPIO_CallbackFxn gpioCallbackFunctions[GPIO_COUNT] = {
   NULL, /* LED Red */
   NULL, /* LED Green */
   NULL, /* RS232 TX enable*/
   NULL, /* PWR_MUX_CTRL */
   NULL, /* LoRa Reset*/
};
