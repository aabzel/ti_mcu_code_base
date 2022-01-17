#include "board_layout.h"

#include <stdlib.h>
#include <ti/drivers/gpio/GPIOCC26XX.h>

#include "data_utils.h"
#include "gpio_drv.h"
#include "sys_config.h"

/*line order does not  matters!*/
const Pin_t PinTable[] = {
 {DIO_LED_GREEN,      12,  0,    "G_LED", NULL, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW, PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MED},
 {DIO_LED_RED,        11,  0,    "R_LED", NULL, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW, PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MED},
 {DIO_UART_CLI_RX,    7,   0,   "MCU_RX", NULL, GPIO_CFG_IN_NOPULL, PIN_INPUT_EN},
 {DIO_UART_CLI_TX,    8,   0,   "MCU_TX", NULL, GPIO_CFG_OUTPUT, PIN_PUSHPULL},
 {DIO_BUTTON1,        19,  0,   "BTN1",   NULL, GPIO_CFG_IN_NOPULL, PIN_INPUT_EN},
 {DIO_BUTTON2,        20,  0,   "BTN2",   NULL, GPIO_CFG_IN_NOPULL, PIN_INPUT_EN},
};

/*line order matters!*/
GPIO_PinConfig gpioPinConfigs[GPIO_COUNT] = {
/*0*/    DIO_LED_GREEN | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW, /* LED Green */
/*1*/    DIO_LED_RED | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW, /* LED Red */
};

/*line order matters!*/
PIN_Config BoardGpioInitTable[GPIO_COUNT + 1] = {
/*0*/    DIO_LED_GREEN | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MED,/* LED Green */
/*1*/    DIO_LED_RED | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MED,/* LED Red */
         PIN_TERMINATE
};

/*line order matters!*/
GPIO_CallbackFxn gpioCallbackFunctions[GPIO_COUNT] = {
/*0*/   NULL, /* LED Red */
/*1*/   NULL, /* LED Green */
};

uint32_t pin_get_cnt(void){
    uint32_t cnt = ARRAY_SIZE(PinTable);
    return cnt;
}

