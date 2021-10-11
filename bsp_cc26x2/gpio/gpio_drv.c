#include "gpio_drv.h"

#include <ioc.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/PIN.h>
#include <ti/drivers/gpio/GPIOCC26XX.h>
#include <ti/drivers/pin/PINCC26XX.h>

#include "bit_utils.h"
#include "board_layout.h"
#include "data_utils.h"

const GPIOCC26XX_Config GPIOCC26XX_config = {.pinConfigs = (GPIO_PinConfig*)gpioPinConfigs,
                                             .callbacks = (GPIO_CallbackFxn*)gpioCallbackFunctions,
                                             .numberOfPinConfigs = 4,
                                             .numberOfCallbacks = 4,
                                             .intPriority = (~0)};

const PINCC26XX_HWAttrs PINCC26XX_hwAttrs = {.intPriority = (~0), .swiPriority = 0};

/*
 *  Callback function for the GPIO interrupt on CONFIG_GPIO_BUTTON_0.
 *  Note: GPIO interrupts are cleared prior to invoking callbacks.
 */
void gpioButtonFxn0(uint_least8_t index) {
    /* Toggle an LED */
    GPIO_toggleDio((uint32_t)DIO_LED_GREEN);
    GPIO_toggleDio((uint32_t)DIO_LED_RED);
}

#if 0
bool gpio_init_layout(const Pin_t* inPinTable, uint8_t size) {
    bool res = false;
    uint8_t pin = 0;

    for(pin = 0; pin < size; pin++) {
        BoardGpioInitTable[pin]=PinTable[pin].pinCfg;
        gpioCallbackFunctions[pin] = PinTable[pin].callBack;
        gpioPinConfigs[pin]=PinTable[pin].pinConfig;
        res = true;
    }
    BoardGpioInitTable[DIO_CNT]= PIN_TERMINATE;
    return res;
}
#endif

inline bool is_dio(uint8_t dio_pin) {
    bool res = false;
    if(dio_pin <= IOID_31) {
        res = true;
    }
    return res;
}
/*
 *  Callback function for the GPIO interrupt on CONFIG_GPIO_BUTTON_1.
 *  This may not be used for all boards.
 *  Note: GPIO interrupts are cleared prior to invoking callbacks.
 */
void gpioButtonFxn1(uint_least8_t index) {
    /* Toggle an LED */
    GPIO_toggleDio((uint32_t)DIO_LED_GREEN);
    GPIO_toggleDio((uint32_t)DIO_LED_RED);
}

GPIO_PinConfig gpio_get_cfg_dio(uint8_t dio_number) {
    GPIO_PinConfig gpio_pin_cfg = GPIO_CFG_IN_NOPULL;
    uint8_t i = 0;
    for(i = 0; i < ARRAY_SIZE(PinTable); i++) {
        if(PinTable[i].dio == dio_number) {
            gpio_pin_cfg = PinTable[i].pinConfig;
            break;
        }
    }
    return gpio_pin_cfg;
}

bool gpio_init(void) {
    bool res = false;
    /*init gpioPinConfigs tructure */
#if 0
    gpio_init_layout(&PinTable[0], DIO_CNT);
#endif
    /* ==== /ti/drivers/PIN initialization ==== */
    if(PIN_init(BoardGpioInitTable) != PIN_SUCCESS) {
        res = false;
    } else {
        res = true;
        GPIO_init();
#ifdef HAS_LED
        GPIO_setConfig(CONF_GPIO_LED_0, gpio_get_cfg_dio(DIO_LED_RED));
        GPIO_setConfig(CONF_GPIO_LED_1, gpio_get_cfg_dio(DIO_LED_GREEN));
#endif /*HAS_LED*/
#ifdef HAS_RS232
        GPIO_setConfig(CONF_GPIO_PS_RS232, gpio_get_cfg_dio(DIO_PS_RS232));
#endif /*HAS_RS232*/
#ifdef HAS_TCAN4550
        GPIO_setConfig(CONF_GPIO_CAN_CS, gpio_get_cfg_dio(DIO_CAN_SS));
        GPIO_setConfig(CONF_GPIO_CAN_RST, gpio_get_cfg_dio(DIO_CAN_RST));
#endif /* HAS_TCAN4550 */

#ifdef HAS_ZED_F9P
        GPIO_setConfig(CONF_GPIO_GNSS_RST_N, gpio_get_cfg_dio(DIO_GNSS_RST_N));
        GPIO_setConfig(CONF_GPIO_GNSS_INT, gpio_get_cfg_dio(DIO_GNSS_INT));
        GPIO_setConfig(CONF_GPIO_GNSS_SAFEBOOT_N, gpio_get_cfg_dio(DIO_GNSS_SAFEBOOT_N));
#endif /* HAS_ZED_F9P */

#ifdef HAS_BOOTLOADER
        GPIO_setConfig(CONF_GPIO_GNSS_RST_N, gpio_get_cfg_dio(DIO_GNSS_RST_N));
#endif /* HAS_BOOTLOADER */

#ifdef HAS_HARVESTER
        GPIO_setConfig(CONF_GPIO_PWR_MUX_CTRL, gpio_get_cfg_dio(DIO_PWR_MUX_CTRL));
        GPIO_setConfig(CONF_GPIO_LEN, gpio_get_cfg_dio(DIO_LEN));
        GPIO_setConfig(CONF_GPIO_BATT_SCL, gpio_get_cfg_dio(DIO_BATT_SCL));
        GPIO_setConfig(CONF_GPIO_BATT_SDA, gpio_get_cfg_dio(DIO_BATT_SDA));
#endif /*HAS_HARVESTER*/

#ifdef LAUNCHXL_CC26X2R1
        GPIO_setConfig(CONFIG_GPIO_BUTTON_0, gpio_get_cfg_dio(DIO_BUTTON_0));

        GPIO_setConfig(CONFIG_GPIO_LOOP_SENSOR, gpio_get_cfg_dio(DIO_LOOP_SENSOR));
        /* Install Button callback */
        GPIO_setCallback(CONFIG_GPIO_BUTTON_0, gpioButtonFxn0);
        /* Enable interrupts */
        GPIO_enableInt(CONFIG_GPIO_BUTTON_0);

        /*
         *  If more than one input pin is available for your device, interrupts
         *  will be enabled on CONFIG_GPIO_BUTTON1.
         */
        if(CONFIG_GPIO_BUTTON_0 != CONFIG_GPIO_BUTTON_1) {
            /* Configure BUTTON1 pin */
            GPIO_setConfig(CONFIG_GPIO_BUTTON_1, gpio_get_cfg_dio(DIO_BUTTON_1));

            /* Install Button callback */
            GPIO_setCallback(CONFIG_GPIO_BUTTON_1, gpioButtonFxn1);
            GPIO_enableInt(CONFIG_GPIO_BUTTON_1);
        }
#endif
#ifdef HAS_LED
        GPIO_writeDio(DIO_LED_RED, 0);
        GPIO_writeDio(DIO_LED_GREEN, 0);
#endif /*HAS_LED*/
#ifdef HAS_RS232
        GPIO_writeDio(DIO_PS_RS232, 1);
#endif /*HAS_RS232*/
#ifdef HAS_TCAN4550
        GPIO_writeDio(DIO_CAN_SS, 1);
        GPIO_writeDio(DIO_CAN_RST, 1);
#endif /* HAS_TCAN4550 */

#ifdef HAS_ZED_F9P
        GPIO_writeDio(DIO_GNSS_SAFEBOOT_N, 1);
        GPIO_writeDio(DIO_GNSS_RST_N, 0);
#endif /* HAS_ZED_F9P */

#ifdef HAS_BOOTLOADER
        GPIO_writeDio(DIO_GNSS_RST_N, 1);
#endif /* HAS_BOOTLOADER */

#ifdef HAS_HARVESTER
        GPIO_writeDio(DIO_GNSS_INT, 1);
        GPIO_writeDio(DIO_LEN, 0);
        GPIO_writeDio(DIO_PWR_MUX_CTRL, 0);
#endif /*HAS_HARVESTER*/
    }
    return res;
}

bool gpio_get_state(uint8_t dio_number, uint8_t* logic_level) {
    uint32_t value = GPIO_readDio((uint32_t)dio_number);
    (*logic_level) = (uint8_t)value;
    return true;
}

bool gpio_set_state(uint8_t dio_number, uint8_t logic_level) {
    GPIO_writeDio(dio_number, logic_level);
    return true;
}

bool is_edge_irq_en(uint8_t dio_pin) {
    bool res = false;
    uint32_t* p_iocfg = (uint32_t*)(IOC_BASE + 4 * dio_pin);
    if((1 << 18) == ((1 << 18) & (*p_iocfg))) {
        res = true;
    }
    return res;
}

bool gpio_set_pull_mode(uint8_t dio_pin, PullMode_t pull_mode) {
    uint32_t ui32Pull = IOC_NO_IOPULL;

    bool res = false;
    res = is_dio(dio_pin);
    if(res) {

        switch(pull_mode) {
        case PULL_DOWN: {
            ui32Pull = IOC_IOPULL_DOWN;
            res = true;
        } break;
        case PULL_UP: {
            ui32Pull = IOC_IOPULL_UP;
            res = true;
        } break;
        case PULL_AIR: {
            ui32Pull = IOC_NO_IOPULL;
            res = true;
        } break;
        default: {
            ui32Pull = IOC_NO_IOPULL;
            res = false;
        } break;
        }
    }
    if(true == res) {
        IOCIOPortPullSet((uint32_t)dio_pin, ui32Pull);
    }

    return res;
}

PullMode_t gpio_get_pull_mode(uint8_t dio_pin) {
    PullMode_t pull_mode = PULL_UNDEF;
    uint32_t* p_iocfg = (uint32_t*)(IOC_BASE + 4 * dio_pin);
    pull_mode = (PullMode_t)extract_subval_from_32bit(*p_iocfg, 14, 13);
    return pull_mode;
}

bool gpio_get_in_mode(uint8_t dio_pin) {
    bool res = false;
    bool is_input = false;
    res = is_dio(dio_pin);
    if(res) {
        uint32_t* p_iocfg = (uint32_t*)(IOC_BASE + 4 * dio_pin);
        is_input = CHECK_BIT_NUM(*p_iocfg, 29);
    }
    return is_input;
}

bool gpio_set_in_mode(uint8_t dio_pin, bool is_in_mode) {
    bool res = false;
    res = is_dio(dio_pin);
    if(res) {
        uint32_t ui32Input = IOC_INPUT_ENABLE;
        if(true == is_in_mode) {
            ui32Input = IOC_INPUT_ENABLE;
            res = true;
        } else {
            ui32Input = IOC_INPUT_DISABLE;
            res = true;
        }
        if(res) {
            IOCIOInputSet((uint32_t)dio_pin, ui32Input);
        }
    }
    return res;
}

uint8_t get_mcu_pin(uint8_t io_pin) {
    uint8_t mcu_pin = 0;
    uint8_t i = 0;
    for(i = 0; i < ARRAY_SIZE(PinTable); i++) {
        if(PinTable[i].dio == io_pin) {
            mcu_pin = PinTable[i].mcu_pin;
            break;
        }
    }
    return mcu_pin;
}

uint8_t get_aux_num(uint8_t io_pin) {
    uint8_t aux_pin = 0;
    uint8_t i = 0;
    for(i = 0; i < ARRAY_SIZE(PinTable); i++) {
        if(PinTable[i].dio == io_pin) {
            aux_pin = PinTable[i].aux_pin;
            break;
        }
    }
    return aux_pin;
}

bool gpio_toggle(uint8_t dio_number) {
    GPIO_toggleDio((uint32_t)dio_number);
    return true;
}

uint32_t gpio_get_alter_fun(uint8_t dio_pin) {
    uint32_t* p_iocfg = (uint32_t*)(IOC_BASE + 4 * dio_pin);
    uint32_t port_id = MASK_5BIT & (*p_iocfg);
    return port_id;
}

DioDir_t gpio_get_dir(uint8_t dio_pin) {
    DioDir_t dir = GPIO_DIR_UNDEF;
    bool is_input = gpio_get_in_mode(dio_pin);
    uint32_t out_en = GPIO_getOutputEnableDio((uint32_t)dio_pin);
    if(is_input) {
        if(GPIO_OUTPUT_ENABLE == out_en) {
            dir = GPIO_DIR_INOUT;
        } else {
            dir = GPIO_DIR_IN;
        }
    } else {
        if(GPIO_OUTPUT_ENABLE == out_en) {
            dir = GPIO_DIR_OUT;
        } else {
            dir = GPIO_DIR_NONE;
        }
    }

    return dir;
}

bool gpio_set_dir(uint8_t dio_pin, DioDir_t des_dir) {
    bool res = false;
    res = is_dio(dio_pin);
    if(res) {
        if(GPIO_DIR_IN == des_dir) {
            IOCIOInputSet((uint32_t)dio_pin, IOC_INPUT_ENABLE);
            GPIO_setOutputEnableDio((uint32_t)dio_pin, GPIO_OUTPUT_DISABLE);
            res = true;
        } else if(GPIO_DIR_OUT == des_dir) {
            GPIO_setOutputEnableDio((uint32_t)dio_pin, GPIO_OUTPUT_ENABLE);
            res = true;
        } else if(GPIO_DIR_INOUT == des_dir) {
            GPIO_setOutputEnableDio((uint32_t)dio_pin, GPIO_OUTPUT_ENABLE);
            res = true;
        } else {
            IOCIOInputSet((uint32_t)dio_pin, IOC_INPUT_DISABLE);
            GPIO_setOutputEnableDio((uint32_t)dio_pin, GPIO_OUTPUT_DISABLE);
            res = true;
        }
    }
    return res;
}
