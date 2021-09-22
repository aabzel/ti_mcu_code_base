#include "gpio_drv.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/PIN.h>
#include <ti/drivers/gpio/GPIOCC26XX.h>
#include <ti/drivers/pin/PINCC26XX.h>

#include "board_layout.h"
#include "bit_utils.h"
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
        GPIO_setConfig(CONF_GPIO_LED_0, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
        GPIO_setConfig(CONF_GPIO_LED_1, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
#endif /*HAS_LED*/
#ifdef HAS_RS232
        GPIO_setConfig(CONF_GPIO_PS_RS232, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
#endif /*HAS_RS232*/
#ifdef  HAS_TCAN4550
        GPIO_setConfig(CONF_GPIO_CAN_CS, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_HIGH);
#endif /* HAS_TCAN4550 */
#ifdef LAUNCHXL_CC26X2R1
        GPIO_setConfig(CONFIG_GPIO_BUTTON_0, GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_FALLING);

        GPIO_setConfig(CONFIG_GPIO_LOOP_SENSOR, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
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
            GPIO_setConfig(CONFIG_GPIO_BUTTON_1, GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_FALLING);

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
#ifdef  HAS_TCAN4550
        GPIO_writeDio(DIO_SS1_CAN, 1);
#endif /* HAS_TCAN4550 */
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
