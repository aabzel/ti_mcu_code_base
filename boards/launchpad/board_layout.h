#ifndef BOARD_LAYOUT_H
#define BOARD_LAYOUT_H

#include <ti/drivers/GPIO.h>
#include <ti/drivers/PIN.h>

#include "sys_config.h"

#define DIO_CNT 4
#define GPIO_COUNT 2U

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

#define LED_COUNT 2
#define CONF_GPIO_LED_0 0
#define CONF_GPIO_LED_1 1
extern const Pin_t PinTable[DIO_CNT];

extern GPIO_PinConfig gpioPinConfigs[GPIO_COUNT];
extern PIN_Config BoardGpioInitTable[GPIO_COUNT + 1];
extern GPIO_CallbackFxn gpioCallbackFunctions[GPIO_COUNT];
uint32_t pin_get_cnt(void);

#endif /* BOARD_LAYOUT_H  */
