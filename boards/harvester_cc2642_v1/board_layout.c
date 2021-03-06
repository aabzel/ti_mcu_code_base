#include "board_layout.h"

#include <stdlib.h>
#include <ti/drivers/gpio/GPIOCC26XX.h>

#include "data_utils.h"
#include "gpio_drv.h"
#include "sys_config.h"

/*line order does not  matters!*/
const Pin_t PinTable[] = {
 {DIO_BATT_ADC,       38,  0, "BATT_ADC", NULL, GPIO_CFG_IN_NOPULL, PIN_INPUT_EN | PIN_NOPULL},
 {DIO_BATT_SCL,       10,  0, "BATT_SCL", NULL, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW,  PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MED},
 {DIO_BATT_SDA,       11,  0, "BATT_SDA", NULL, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW,  PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MED},
 {DIO_CAN_INT,        14,  0,  "CAN_INT", NULL, GPIO_CFG_IN_NOPULL, PIN_INPUT_EN | PIN_NOPULL},
 {DIO_CAN_RST,        15,  0,  "CAN_RST", NULL, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW, PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MED},
 {DIO_CAN_SS,          6,  0,   "CAN_SS", NULL, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_HIGH, PIN_GPIO_OUTPUT_EN | PIN_GPIO_HIGH | PIN_PUSHPULL | PIN_DRVSTR_MED},
 {DIO_GNSS_INT,       30,  0, "GNSS_Int", NULL, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_HIGH | GPIO_CFG_OUT_HIGH, PIN_GPIO_OUTPUT_EN | PIN_GPIO_HIGH | PIN_PUSHPULL | PIN_DRVSTR_MAX},
 {DIO_GNSS_RST_N,     28,  0, "GNSS_Rst", NULL, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW, PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MED},
 {DIO_GNSS_RXD,       17,  0, "GNSS_RXD", NULL, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW,  PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MED},
 {DIO_GNSS_SAFEBOOT_N,29,  0,"GNSS_SaBo", NULL, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW, PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MED},
 {DIO_GNSS_TXD,       16,  0, "GNSS_TXD", NULL, GPIO_CFG_IN_NOPULL, PIN_INPUT_EN | PIN_NOPULL},
 {DIO_KL30_ADC,       37,  0, "KL30_ADC", NULL, GPIO_CFG_IN_NOPULL, PIN_INPUT_EN | PIN_NOPULL},
 {DIO_LED_GREEN,      43,  0,    "G_LED", NULL, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW, PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MED},
 {DIO_LED_RED,        39,  0,    "R_LED", NULL, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW, PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MED},
 {DIO_LEN,            32,  0,      "LEN", NULL, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW, PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MED},
 {DIO_MISO,            7,  0,     "MISO", NULL, GPIO_CFG_IN_NOPULL, PIN_INPUT_EN | PIN_NOPULL},
 {DIO_MOSI,            8,  0,     "MOSI", NULL, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW,  PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MED},
 {DIO_PS_RS232,       12,  0, "PS_RS232", NULL, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW, PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MED},
 {DIO_PWR_MUX_CTRL,   20,  0,   "PwrMux", NULL, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW, PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MED},
 {DIO_SCLK,            9,  0,     "SCLK", NULL, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW,  PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MED},
 {DIO_SX1262_BUSY,    42,  0,  "LoRaBsy", NULL, GPIO_CFG_IN_NOPULL, PIN_INPUT_EN},
 {DIO_SX1262_RST,     40,  0,  "LoRaRst", NULL, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW,  PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MED},
 {DIO_SX1262_SS,       5,  0,   "LoRaCC", NULL, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW,  PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MED},
 {DIO_UART_CLI_TX,  18,  0,   "MCU_TX", NULL, GPIO_CFG_OUTPUT, PIN_PUSHPULL},
 {DIO_3V3_CTRL,       31,  0, "3V3_CTRL", NULL, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW, PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MED},
 {DIO_UART_CLI_RX,  19,  0,   "MCU_RX", NULL, GPIO_CFG_IN_NOPULL , PIN_INPUT_EN},
 {DIO_TDO,  26,  0,      "TDO", NULL, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW, PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MED},
 {DIO_TDI,  27,  0,      "TDI", NULL, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW, PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MED},
};

/*line order matters!*/
GPIO_PinConfig gpioPinConfigs[GPIO_COUNT] = {
/*0*/    DIO_LED_GREEN |    GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW, /* LED Green */
/*1*/    DIO_LED_RED |      GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW, /* LED Red */
/*2*/    DIO_PS_RS232 |     GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_HIGH,  /*RS232 TX enable*/
/*3*/    DIO_PWR_MUX_CTRL | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW, /* PWR_MUX_CTRL */
/*4*/    DIO_SX1262_SS |    GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_HIGH, /*LoRa CS*/
/*5*/    DIO_SX1262_RST |    GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_HIGH, /*LoRa Reset*/
/*6*/    DIO_SX1262_INT |   GPIO_CFG_IN_NOPULL,                                          /*LoRa int*/
/*7*/    DIO_SX1262_BUSY |  GPIO_CFG_IN_NOPULL,                                          /*LoRa Busy*/
/*8*/    DIO_CAN_SS |       GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_HIGH, /*CAN chip select*/
/*9*/    DIO_GNSS_RST_N  |  GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW, /* RST_N_GNSS */
/*10*/   DIO_GNSS_INT |        GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_HIGH, /* GNSS_INT */
/*11*/   DIO_GNSS_SAFEBOOT_N | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_HIGH, /* GNSS_SAFEBOOT_N */
/*12*/   DIO_LEN |             GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW, /* LEN */
/*13*/   DIO_BATT_SCL |        GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW , /* BATT_SCL */
/*14*/   DIO_BATT_SDA |        GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW , /* BATT_SDA */
/*15*/   DIO_CAN_RST |         GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW , /* CAN_RST */
/*16*/   DIO_SCLK |    GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW,
/*17*/   DIO_MOSI |    GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW,
/*18*/   DIO_MISO |    GPIO_CFG_IN_NOPULL,
/*19*/   DIO_GNSS_RXD | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MED,
/*20*/   DIO_GNSS_TXD | GPIO_CFG_IN_NOPULL ,
/*21*/   DIO_CAN_INT |  GPIO_CFG_IN_NOPULL ,
/*22*/   DIO_3V3_CTRL | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW, /* 3V3_CTRL */
};

/*line order matters!*/
PIN_Config BoardGpioInitTable[GPIO_COUNT + 1] = {
/*0*/    DIO_LED_GREEN | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MED,/* LED Green */
/*1*/    DIO_LED_RED |   PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MED,/* LED Red */
/*2*/    DIO_PS_RS232 |  PIN_GPIO_OUTPUT_EN | PIN_GPIO_HIGH | PIN_PUSHPULL | PIN_DRVSTR_MED,/*RS232 TX enable*/
/*3*/    DIO_PWR_MUX_CTRL | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MED,/* PWR_MUX_CTRL */
/*4*/    DIO_SX1262_SS  |   PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MED,  /*LoRa CS*/
/*5*/    DIO_SX1262_RST  |  PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MED, /*LoRa Reset*/
/*6*/    DIO_SX1262_INT  | PIN_INPUT_EN,                           /*LoRa Int*/
/*7*/    DIO_SX1262_BUSY  | PIN_INPUT_EN,                          /*LoRa Busy*/
/*8*/    DIO_CAN_SS |       PIN_GPIO_OUTPUT_EN | PIN_GPIO_HIGH| PIN_PUSHPULL | PIN_DRVSTR_MED,
/*9*/    DIO_GNSS_RST_N |   PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MED,/* RST_N_GNSS */
/*10*/   DIO_GNSS_INT |     PIN_GPIO_OUTPUT_EN | PIN_GPIO_HIGH | PIN_PUSHPULL | PIN_DRVSTR_MED,/* GNSS_INT */
/*11*/   DIO_GNSS_SAFEBOOT_N | PIN_GPIO_OUTPUT_EN | PIN_GPIO_HIGH | PIN_PUSHPULL | PIN_DRVSTR_MED,/* GNSS_SAFEBOOT_N */
/*12*/   DIO_LEN |             PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MED,/* LEN */
/*13*/   DIO_BATT_SCL | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MED,/* BATT_SCL */
/*14*/   DIO_BATT_SDA | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MED,/* BATT_SDA */
/*15*/   DIO_CAN_RST |  PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MED,/* CAN_RST */
/*16*/   DIO_SCLK | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MED,
/*17*/   DIO_MOSI | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MED,
/*18*/   DIO_MISO | PIN_INPUT_EN,
/*19*/   DIO_GNSS_RXD | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MED,
/*20*/   DIO_GNSS_TXD | PIN_INPUT_EN,
/*21*/   DIO_CAN_INT |  PIN_INPUT_EN,
/*22*/   DIO_3V3_CTRL | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MED,/*3V3_CTRL*/
         PIN_TERMINATE
};

/*line order matters!*/
GPIO_CallbackFxn gpioCallbackFunctions[GPIO_COUNT] = {
/*0*/   NULL, /* LED Red */
/*1*/   NULL, /* LED Green */
/*2*/   NULL, /* RS232 TX enable*/
/*3*/   NULL, /* PWR_MUX_CTRL */
/*4*/   NULL, /* LoRa CS*/
/*5*/   NULL, /* LoRa Reset*/
/*6*/   NULL, /* LoRa int*/
/*7*/   NULL, /* LoRa Busy*/
/*8*/   NULL,
/*9*/   NULL,
/*10*/  NULL,
/*11*/  NULL,
/*12*/  NULL,
/*13*/  NULL,
/*14*/  NULL,
/*15*/  NULL,/* CAN_RST */
/*16*/  NULL,
/*17*/  NULL,
/*18*/  NULL,
/*19*/  NULL,
/*20*/  NULL,
/*21*/  NULL,
/*22*/  NULL
};


bool io_bang_gpio_init(void){
    GPIO_setConfig(CONF_GPIO_SCLK, gpio_get_cfg_dio(DIO_SCLK));
    GPIO_setConfig(CONF_GPIO_MOSI, gpio_get_cfg_dio(DIO_MOSI));
    GPIO_setConfig(CONF_GPIO_PS_RS232, gpio_get_cfg_dio(DIO_PS_RS232));
    GPIO_setConfig(CONF_GPIO_GNSS_RXD, gpio_get_cfg_dio(DIO_GNSS_RXD));
    GPIO_setConfig(CONF_GPIO_PWR_MUX_CTRL, gpio_get_cfg_dio(DIO_PWR_MUX_CTRL));
    GPIO_setConfig(CONF_GPIO_BATT_SCL , gpio_get_cfg_dio(DIO_GNSS_RXD));
    GPIO_setConfig(CONF_GPIO_BATT_SDA, gpio_get_cfg_dio(DIO_BATT_SDA));
    GPIO_setConfig(CONF_GPIO_GNSS_RST_N, gpio_get_cfg_dio(DIO_GNSS_RST_N));

    GPIO_writeDio(DIO_GNSS_RST_N, 1);
    GPIO_writeDio(DIO_SCLK, 0);
    GPIO_writeDio(DIO_MOSI, 0);
    return true;
}

uint32_t pin_get_cnt(void){
    uint32_t cnt = ARRAY_SIZE(PinTable);
    return cnt;
}
