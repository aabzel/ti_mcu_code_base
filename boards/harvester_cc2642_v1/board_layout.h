#ifndef BOARD_LAYOUT_H
#define BOARD_LAYOUT_H

#include <ti/drivers/GPIO.h>
#include <ti/drivers/PIN.h>

#include "sys_config.h"

#ifndef CC26XX
#error "that wile only for CC26CC MCUs"
#endif /*USE_HAL_DRIVER*/

#define DIO_CNT 28U
#define GPIO_COUNT 22U

#define PIN_NAME_LEN 10U
typedef struct xPin_t {
    uint8_t dio;
    uint8_t mcu_pin;
    uint8_t aux_pin;
    char name[PIN_NAME_LEN];
    GPIO_CallbackFxn callBack;
    GPIO_PinConfig pinConfig;
    PIN_Config pinCfg;
} Pin_t;


#define CONF_GPIO_BATT_SCL 13U
#define CONF_GPIO_BATT_SDA 14U
#define CONF_GPIO_CAN_CS 8U
#define CONF_GPIO_CAN_RST 15U
#define CONF_GPIO_GNSS_INT 10U
#define CONF_GPIO_GNSS_RST_N 9U
#define CONF_GPIO_GNSS_RXD 19U
#define CONF_GPIO_GNSS_SAFEBOOT_N 11U
#define CONF_GPIO_GNSS_TXD 20U
#define CONF_GPIO_LED_0 0U
#define CONF_GPIO_LED_1 1U
#define CONF_GPIO_LEN 12U
#define CONF_GPIO_LORA_BSY 7U
#define CONF_GPIO_LORA_CS 4U
#define CONF_GPIO_LORA_INT 6U
#define CONF_GPIO_LORA_RST 5U
#define CONF_GPIO_MISO 18U
#define CONF_GPIO_MOSI 17U
#define CONF_GPIO_PS_RS232 2U
#define CONF_GPIO_PWR_MUX_CTRL 3U
#define CONF_GPIO_SCLK 16U

extern const Pin_t PinTable[DIO_CNT];

extern GPIO_PinConfig gpioPinConfigs[GPIO_COUNT];
extern PIN_Config BoardGpioInitTable[GPIO_COUNT + 1];
extern GPIO_CallbackFxn gpioCallbackFunctions[GPIO_COUNT];

bool io_bang_gpio_init(void);

#endif /* BOARD_LAYOUT_H  */
