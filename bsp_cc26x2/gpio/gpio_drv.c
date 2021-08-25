#include "gpio_drv.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/PIN.h>
#include <ti/drivers/gpio/GPIOCC26XX.h>
#include <ti/drivers/pin/PINCC26XX.h>

#include "bit_utils.h"
#include "data_utils.h"

const uint_least8_t CONFIG_GPIO_BUTTON_0_CONST = CONFIG_GPIO_BUTTON_0;
const uint_least8_t CONFIG_GPIO_BUTTON_1_CONST = CONFIG_GPIO_BUTTON_1;
const uint_least8_t CONFIG_GPIO_LED_0_CONST = CONFIG_GPIO_LED_0;
const uint_least8_t CONFIG_GPIO_LED_1_CONST = CONFIG_GPIO_LED_1;

GPIO_PinConfig gpioPinConfigs[CONFIG_GPIO_COUNT] = {
    GPIOCC26XX_DIO_13 | GPIO_DO_NOT_CONFIG, /* CONFIG_GPIO_BUTTON_0 : LaunchPad Button BTN-1 (Left) */
    GPIOCC26XX_DIO_14 | GPIO_DO_NOT_CONFIG, /* CONFIG_GPIO_BUTTON_1 : LaunchPad Button BTN-2 (Right) */

    GPIOCC26XX_DIO_06 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED |
        GPIO_CFG_OUT_LOW, /* CONFIG_GPIO_LED_0 : LaunchPad LED Red */
    GPIOCC26XX_DIO_07 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED |
        GPIO_CFG_OUT_LOW, /* CONFIG_GPIO_LED_1 : LaunchPad LED Green */

    GPIOCC26XX_DIO_11 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_HIGH, /*LoRa CS*/
    GPIOCC26XX_DIO_27 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_HIGH, /*LoRa Reset*/
    GPIOCC26XX_DIO_28 | GPIO_CFG_IN_NOPULL,                                          /*LoRa int*/
    GPIOCC26XX_DIO_29 | GPIO_CFG_IN_NOPULL,                                          /*LoRa Busy*/

};

/*
 *  ======== gpioCallbackFunctions ========
 *  Array of callback function pointers
 *
 *  NOTE: Unused callback entries can be omitted from the callbacks array to
 *  reduce memory usage by enabling callback table optimization
 *  (GPIO.optimizeCallbackTableSize = true)
 */
GPIO_CallbackFxn gpioCallbackFunctions[CONFIG_GPIO_COUNT] = {
    NULL, /* CONFIG_GPIO_BUTTON_0 : LaunchPad Button BTN-1 (Left) */
    NULL, /* CONFIG_GPIO_BUTTON_1 : LaunchPad Button BTN-2 (Right) */
    NULL, /* CONFIG_GPIO_LED_0 : LaunchPad LED Red */
    NULL, /* CONFIG_GPIO_LED_1 : LaunchPad LED Green */
    NULL, /*LoRa CS*/
    NULL, /*LoRa Reset*/
    NULL, /*LoRa int*/
    NULL  /*LoRa Busy*/
};

/*
 *  ======== GPIOCC26XX_config ========
 */
const GPIOCC26XX_Config GPIOCC26XX_config = {.pinConfigs = (GPIO_PinConfig*)gpioPinConfigs,
                                             .callbacks = (GPIO_CallbackFxn*)gpioCallbackFunctions,
                                             .numberOfPinConfigs = 4,
                                             .numberOfCallbacks = 4,
                                             .intPriority = (~0)};

PIN_Config BoardGpioInitTable[CONFIG_GPIO_COUNT + 1] = {
    /* LaunchPad Button BTN-1 (Left), Parent Signal: CONFIG_GPIO_BUTTON_0 GPIO Pin, (DIO13) */
    CONFIG_PIN_0 | PIN_INPUT_EN | PIN_PULLUP | PIN_IRQ_DIS,
    /* LaunchPad Button BTN-2 (Right), Parent Signal: CONFIG_GPIO_BUTTON_1 GPIO Pin, (DIO14) */
    CONFIG_PIN_1 | PIN_INPUT_EN | PIN_PULLUP | PIN_IRQ_DIS,
    /* LaunchPad LED Red, Parent Signal: CONFIG_GPIO_LED_0 GPIO Pin, (DIO6) */
    CONFIG_PIN_2 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MED,
    /* LaunchPad LED Green, Parent Signal: CONFIG_GPIO_LED_1 GPIO Pin, (DIO7) */
    CONFIG_PIN_3 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MED,
    SX1262_SS_DIO_NO | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MED,  /*LoRa CS*/
    SX1262_RST_DIO_NO | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MED, /*LoRa Reset*/
    SX1262_INT_DIO_NO | PIN_INPUT_EN | PIN_NOPULL | PIN_IRQ_DIS,                           /*LoRa Int*/
    SX1262_BUSY_DIO_NO | PIN_INPUT_EN | PIN_NOPULL | PIN_IRQ_DIS,                          /*LoRa Busy*/
    PIN_TERMINATE};

const PINCC26XX_HWAttrs PINCC26XX_hwAttrs = {.intPriority = (~0), .swiPriority = 0};

/*
 *  Callback function for the GPIO interrupt on CONFIG_GPIO_BUTTON_0.
 *  Note: GPIO interrupts are cleared prior to invoking callbacks.
 */
void gpioButtonFxn0(uint_least8_t index) {
    /* Toggle an LED */
    GPIO_toggle(CONFIG_GPIO_LED_0);
    GPIO_toggle(CONFIG_GPIO_LED_1);
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
    GPIO_toggle(CONFIG_GPIO_LED_1);
    GPIO_toggle(CONFIG_GPIO_LED_0);
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

        /* Configure the LED and button pins */
        GPIO_setConfig(CONFIG_GPIO_LED_0, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
        GPIO_setConfig(CONFIG_GPIO_LED_1, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
        GPIO_setConfig(CONFIG_GPIO_BUTTON_0, GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_FALLING);

        /* Turn on user LED */
        GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_ON);

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
        GPIO_write(CONFIG_GPIO_LED_0, 0);
        GPIO_write(CONFIG_GPIO_LED_1, 0);
    }
    return res;
}

bool gpio_get_state(uint8_t dioNumber, uint8_t* logic_level) {
    uint32_t value = GPIO_readDio((uint32_t)dioNumber);
    (*logic_level) = (uint8_t)value;
    return true;
}

bool gpio_set_state(uint8_t dioNumber, uint8_t logic_level) {
    GPIO_writeDio(dioNumber, logic_level);
#if 0
    GPIO_write((uint_least8_t)port_pin_num, (unsigned int)logic_level);
#endif
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

bool gpio_toggle(uint8_t dioNumber) {
    GPIO_toggleDio((uint32_t)dioNumber);
#if 0
    GPIO_toggle((uint_least8_t)pin);
#endif
    return true;
}
