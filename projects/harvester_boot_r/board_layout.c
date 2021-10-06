#include "board_layout.h"

#include <stdlib.h>
#include <ti/drivers/gpio/GPIOCC26XX.h>

#include "gpio_drv.h"
#include "sys_config.h"

/*line order does not  matters!*/
const Pin_t PinTable[DIO_CNT] = {
 {DIO_BATT_SCL,       10,  0, "BATT_SCL", NULL, GPIO_CFG_IN_NOPULL, PIN_INPUT_EN | PIN_NOPULL},
 {DIO_BATT_SDA,       11,  0, "BATT_SDA", NULL, GPIO_CFG_IN_NOPULL, PIN_INPUT_EN | PIN_NOPULL},
 {DIO_PS_RS232,       12,  0, "PS_RS232", NULL, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW, PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MED},
 {GPIOCC26XX_DIO_12,  18,  0,   "MCU_TX", NULL, GPIO_CFG_OUTPUT, PIN_PUSHPULL},
 {GPIOCC26XX_DIO_13,  19,  0,   "MCU_RX", NULL, GPIO_CFG_IN_NOPULL, PIN_INPUT_EN},
 {DIO_PWR_MUX_CTRL,   20,  0,   "PwrMux", NULL, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW, PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MED},
 {GPIOCC26XX_DIO_16,  26,  0,      "TDO", NULL, GPIO_CFG_OUTPUT, PIN_INPUT_EN},
 {GPIOCC26XX_DIO_17,  27,  0,      "TDI", NULL, GPIO_CFG_INPUT,  PIN_INPUT_EN},
 {DIO_GNSS_RST_N,     28,  0, "GNSS_Rst", NULL, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW,   PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MED},
 {DIO_GNSS_INT,       30,  0, "GNSS_Int", NULL, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_HIGH,      PIN_GPIO_OUTPUT_EN | PIN_GPIO_HIGH | PIN_PUSHPULL | PIN_DRVSTR_MED},
 {DIO_LEN,            32,  0,      "LEN", NULL, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW, PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MED},
 {DIO_LED_RED,        39,  0,    "R_LED", NULL, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW, PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MED},
 {DIO_LED_GREEN,      43,  0,    "G_LED", NULL, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW, PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MED},
};

/*line order matters!*/
GPIO_PinConfig gpioPinConfigs[GPIO_COUNT] = {
/*0*/    DIO_LED_GREEN | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW, /* LED Green */
/*1*/    DIO_LED_RED | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW, /* LED Red */
/*2*/    DIO_PS_RS232 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_HIGH,  /*RS232 TX enable*/
/*3*/    DIO_PWR_MUX_CTRL | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW, /* PWR_MUX_CTRL */
/*4*/    DIO_GNSS_RST_N | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW, /* RST_N_GNSS */
/*5*/    DIO_LEN | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW, /* LEN */
/*6*/    DIO_GNSS_INT | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_HIGH, /* GNSS_INT */
/*7*/    DIO_BATT_SCL | GPIO_CFG_IN_NOPULL , /* BATT_SCL */
/*8*/    DIO_BATT_SDA | GPIO_CFG_IN_NOPULL , /* BATT_SDA */
};

/*line order matters!*/
PIN_Config BoardGpioInitTable[GPIO_COUNT + 1] = {
/*0*/    DIO_LED_GREEN | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MED,/* LED Green */
/*1*/    DIO_LED_RED | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MED,/* LED Red */
/*2*/    DIO_PS_RS232 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_HIGH | PIN_PUSHPULL | PIN_DRVSTR_MED,/*RS232 TX enable*/
/*3*/    DIO_PWR_MUX_CTRL | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MED,/* PWR_MUX_CTRL */
/*4*/    DIO_GNSS_RST_N | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MED,/* RST_N_GNSS */
/*5*/    DIO_LEN | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MED,/* LEN */
/*6*/    DIO_GNSS_INT | PIN_GPIO_OUTPUT_EN | PIN_GPIO_HIGH | PIN_PUSHPULL | PIN_DRVSTR_MED,/* GNSS_INT */
/*7*/    DIO_BATT_SCL | PIN_INPUT_EN | PIN_NOPULL,/* BATT_SCL */
/*8*/    DIO_BATT_SDA | PIN_INPUT_EN | PIN_NOPULL,/* BATT_SDA */

    PIN_TERMINATE
};

/*line order matters!*/
GPIO_CallbackFxn gpioCallbackFunctions[GPIO_COUNT] = {
/*0*/   NULL, /* LED Red */
/*1*/   NULL, /* LED Green */
/*2*/   NULL, /* RS232 TX enable*/
/*3*/   NULL, /* PWR_MUX_CTRL */
/*4*/   NULL, /* GNSS Reset*/
/*5*/   NULL, /* LEN*/
/*6*/   NULL, /*GNSS_INT*/
/*7*/   NULL, /*BATT_SCL*/
/*8*/   NULL, /*BATT_SDA*/
};