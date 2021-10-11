#ifndef BOARD_LAYOUT_H
#define BOARD_LAYOUT_H

#include <ti/drivers/GPIO.h>
#include <ti/drivers/PIN.h>

#include "sys_config.h"

#define DIO_CNT 24
#define GPIO_COUNT 15U

#define PIN_NAME_LEN 10
typedef struct xPin_t {
    uint8_t dio;
    uint8_t mcu_pin;
    uint8_t aux_pin;
    char name[PIN_NAME_LEN];
    GPIO_CallbackFxn callBack;
    GPIO_PinConfig pinConfig;
    PIN_Config pinCfg;
} Pin_t;


#define CONF_GPIO_BATT_SCL 13
#define CONF_GPIO_BATT_SDA 14
#define CONF_GPIO_LED_0 0
#define CONF_GPIO_LED_1 1
#define CONF_GPIO_LEN 12
#define CONF_GPIO_LORA_CS 4
#define CONF_GPIO_LORA_RST 5
#define CONF_GPIO_LORA_INT 6
#define CONF_GPIO_LORA_BSY 7
#define CONF_GPIO_CAN_CS 8
#define CONF_GPIO_GNSS_RST_N 9
#define CONF_GPIO_GNSS_INT 10
#define CONF_GPIO_GNSS_SAFEBOOT_N 11
#define CONF_GPIO_PS_RS232 2
#define CONF_GPIO_PWR_MUX_CTRL 3

extern const Pin_t PinTable[DIO_CNT];

extern GPIO_PinConfig gpioPinConfigs[GPIO_COUNT];
extern PIN_Config BoardGpioInitTable[GPIO_COUNT + 1];
extern GPIO_CallbackFxn gpioCallbackFunctions[GPIO_COUNT];

#endif /* BOARD_LAYOUT_H  */
