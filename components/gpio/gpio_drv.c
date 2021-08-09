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

GPIO_PinConfig gpioPinConfigs[] = {
    /* CONFIG_GPIO_BUTTON_0 : LaunchPad Button BTN-1 (Left) */
    GPIOCC26XX_DIO_13 | GPIO_DO_NOT_CONFIG,
    /* CONFIG_GPIO_BUTTON_1 : LaunchPad Button BTN-2 (Right) */
    GPIOCC26XX_DIO_14 | GPIO_DO_NOT_CONFIG,
    /* CONFIG_GPIO_LED_0 : LaunchPad LED Red */
    GPIOCC26XX_DIO_06 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW,
    /* CONFIG_GPIO_LED_1 : LaunchPad LED Green */
    GPIOCC26XX_DIO_07 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW,
};

/*
 *  ======== gpioCallbackFunctions ========
 *  Array of callback function pointers
 *
 *  NOTE: Unused callback entries can be omitted from the callbacks array to
 *  reduce memory usage by enabling callback table optimization
 *  (GPIO.optimizeCallbackTableSize = true)
 */
GPIO_CallbackFxn gpioCallbackFunctions[] = {
    /* CONFIG_GPIO_BUTTON_0 : LaunchPad Button BTN-1 (Left) */
    NULL,
    /* CONFIG_GPIO_BUTTON_1 : LaunchPad Button BTN-2 (Right) */
    NULL,
    /* CONFIG_GPIO_LED_0 : LaunchPad LED Red */
    NULL,
    /* CONFIG_GPIO_LED_1 : LaunchPad LED Green */
    NULL,
};

/*
 *  ======== GPIOCC26XX_config ========
 */
const GPIOCC26XX_Config GPIOCC26XX_config = {.pinConfigs = (GPIO_PinConfig*)gpioPinConfigs,
                                             .callbacks = (GPIO_CallbackFxn*)gpioCallbackFunctions,
                                             .numberOfPinConfigs = 4,
                                             .numberOfCallbacks = 4,
                                             .intPriority = (~0)};

#define CONFIG_PIN_COUNT 4

const PIN_Config BoardGpioInitTable[CONFIG_PIN_COUNT + 1] = {
    /* LaunchPad Button BTN-1 (Left), Parent Signal: CONFIG_GPIO_BUTTON_0 GPIO Pin, (DIO13) */
    CONFIG_PIN_0 | PIN_INPUT_EN | PIN_PULLUP | PIN_IRQ_DIS,
    /* LaunchPad Button BTN-2 (Right), Parent Signal: CONFIG_GPIO_BUTTON_1 GPIO Pin, (DIO14) */
    CONFIG_PIN_1 | PIN_INPUT_EN | PIN_PULLUP | PIN_IRQ_DIS,
    /* LaunchPad LED Red, Parent Signal: CONFIG_GPIO_LED_0 GPIO Pin, (DIO6) */
    CONFIG_PIN_2 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MED,
    /* LaunchPad LED Green, Parent Signal: CONFIG_GPIO_LED_1 GPIO Pin, (DIO7) */
    CONFIG_PIN_3 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MED,

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
    /* ==== /ti/drivers/PIN initialization ==== */
    if(PIN_init(BoardGpioInitTable) != PIN_SUCCESS) {
        /* Error with PIN_init */
        while(1)
            ;
    }

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
    return true;
}

bool gpio_get_state(uint8_t port_pin_num, uint8_t* logic_level) {
    uint_fast8_t value = GPIO_read((uint_least8_t)port_pin_num);
    *logic_level = (uint8_t)value;
    return true;
}

bool gpio_set_state(uint8_t port_pin_num, uint8_t logic_level) {
    GPIO_write((uint_least8_t)port_pin_num, (unsigned int)logic_level);
    return true;
}

/**/
const char* get_gpio_mode(uint8_t pin) {
    char* name = "_";
    GPIO_PinConfig pin_cfg = 0;
    if(pin < CONFIG_TI_DRIVERS_PIN_COUNT) {
        GPIO_getConfig((uint_least8_t)pin, &pin_cfg);
    }
    return name;
}

static const char* port_ip2str(uint8_t code) {
    char* name = "no";
    switch(code) {
    case 0x0:
        name = "GP IO";
        break;
    case 0x7:
        name = "AON 32 KHz";
        break;
    case 0x8:
        name = "AUX IO";
        break;
    case 0x9:
        name = "SSI0_RX";
        break;
    case 0xA:
        name = "SSI0_TX";
        break;
    case 0xB:
        name = "SSI0_FSS";
        break;
    case 0xC:
        name = "SSI0_CLK";
        break;
    case 0xD:
        name = "I2C_MSSDA";
        break;
    case 0xE:
        name = "I2C_MSSCL";
        break;
    case 0xF:
        name = "UART0_RX";
        break;
    case 0x10:
        name = "UART0_TX";
        break;
    case 0x11:
        name = "UART0_CTS";
        break;
    case 0x12:
        name = "UART0_RTS";
        break;
    case 0x13:
        name = "UART1_RX";
        break;
    case 0x14:
        name = "UART1_TX";
        break;
    case 0x15:
        name = "UART1_CTS";
        break;
    case 0x16:
        name = "UART1_RTS";
        break;
    case 0x17:
        name = "PORT_EVENT0";
        break;
    case 0x18:
        name = "PORT_EVENT1";
        break;
    case 0x19:
        name = "PORT_EVENT2";
        break;
    case 0x1A:
        name = "PORT_EVENT3";
        break;
    case 0x1B:
        name = "PORT_EVENT4";
        break;
    case 0x1C:
        name = "PORT_EVENT5";
        break;
    case 0x1D:
        name = "PORT_EVENT6";
        break;
    case 0x1E:
        name = "PORT_EVENT7";
        break;
    case 0x20:
        name = "CPU_SWV";
        break;
    case 0x21:
        name = "SSI1_RX";
        break;
    case 0x22:
        name = "SSI1_TX";
        break;
    case 0x23:
        name = "SSI1_FSS";
        break;
    case 0x24:
        name = "SSI1_CLK";
        break;
    case 0x25:
        name = "I2S_AD0";
        break;
    case 0x26:
        name = "I2S_AD1";
        break;
    case 0x27:
        name = "I2S_WCLK";
        break;
    case 0x28:
        name = "I2S_BCLK";
        break;
    case 0x29:
        name = "I2S_MCLK";
        break;
    case 0x2E:
        name = "RfCoreTrace";
        break;
    case 0x2F:
        name = "RfCoreDataOut0";
        break;
    case 0x30:
        name = "RfCoreDataOut1";
        break;
    case 0x31:
        name = "RfCoreDataOut2";
        break;
    case 0x32:
        name = "RfCoreDataOut3";
        break;
    case 0x33:
        name = "RfCoreDataIn0";
        break;
    case 0x34:
        name = "RfCoreDataIn1";
        break;
    case 0x35:
        name = "RfCoreSMIDataLinkOut";
        break;
    case 0x36:
        name = "RfCoreSMIDataLinkIn";
        break;
    case 0x37:
        name = "RfCoreSMICmdLinkOut";
        break;
    case 0x38:
        name = "RfCoreSMICmdLinkIn";
        break;
    default:
        name = "undef";
        break;
    }
    return name;
}

const char* get_gpio_alter_fun(uint8_t dio_pin) {
    const char* name = "no";
    uint32_t* p_iocfg = (uint32_t*)(IOC_BASE + 4 * dio_pin);
    uint32_t port_id = MASK_5BIT & (*p_iocfg);
    name = port_ip2str(port_id);
    return name;
}

const char* get_gpio_pull_mode(uint8_t dio_pin) {
    char* pull_name = "no";
    uint32_t* p_iocfg = (uint32_t*)(IOC_BASE + 4 * dio_pin);
    uint32_t pull_code = extract_subval_from_32bit(*p_iocfg, 14, 13);
    switch(pull_code) {
    case 1:
        pull_name = "Down";
        break;
    case 2:
        pull_name = " Up ";
        break;
    case 3:
        pull_name = "Air ";
        break;
    default:
        pull_name = "Err ";
        break;
    }
    return pull_name;
}

static const char* iomode2str(uint8_t code) {
    const char* name = "no";
    switch(code) {
    case 0:
        name = "NormIO";
        break;
    case 1:
        name = "InvIO";
        break;
    case 4:
        name = "NormOpenDr";
        break;
    case 5:
        name = "InvOpenDr";
        break;
    case 6:
        name = "OpenSrc";
        break;
    case 7:
        name = "InvOpenSrc";
        break;
    default:
        name = "rsvd";
        break;
    }
    return name;
}

bool is_edge_irq_en(uint8_t dio_pin) {
    bool res = false;
    uint32_t* p_iocfg = (uint32_t*)(IOC_BASE + 4 * dio_pin);
    if((1 << 18) == ((1 << 18) & (*p_iocfg))) {
        res = true;
    }
    return res;
}

const char* get_gpio_type(uint8_t dio_pin) {
    const char* iomode_name = "no";
    uint32_t* p_iocfg = (uint32_t*)(IOC_BASE + 4 * dio_pin);
    uint32_t iomode_code = extract_subval_from_32bit(*p_iocfg, 26, 24);
    iomode_name = iomode2str(iomode_code);
    return iomode_name;
}

static const char* edge2str(uint8_t edge_code) {
    const char* edge_name = "no";
    switch(edge_code) {
    case 0:
        edge_name = "none";
        break;
    case 1:
        edge_name = "neg ";
        break;
    case 2:
        edge_name = "pos ";
        break;
    case 3:
        edge_name = "both";
        break;

    default:
        edge_name = "err ";
        break;
    }
    return edge_name;
}

const char* get_gpio_edge(uint8_t dio_pin) {
    const char* edge_name = "no";
    uint32_t* p_iocfg = (uint32_t*)(IOC_BASE + 4 * dio_pin);
    uint32_t edge_code = extract_subval_from_32bit(*p_iocfg, 17, 16);
    edge_name = edge2str(edge_code);
    return edge_name;
}

// 13.3.2 Mapping AUXIOs to DIO Pins
const Pin_t PinTable[DIO_CNT] = {
    {0, 5, 18},   {1, 6, 17},   {2, 7, 16},   {3, 8, 15},   {4, 9, 14},   {5, 10, 13},  {6, 11, 12},  {7, 12, 11},
    {8, 14, 10},  {9, 15, 9},   {10, 16, 8},  {11, 17, 7},  {12, 18, 6},  {13, 19, 5},  {14, 20, 4},  {15, 21, 3},
    {16, 26, 2},  {17, 27, 1},  {18, 28, 31}, {19, 29, 30}, {20, 30, 29}, {21, 31, 28}, {22, 32, 27}, {23, 36, 26},
    {24, 37, 25}, {25, 38, 24}, {26, 39, 23}, {27, 40, 22}, {28, 41, 21}, {29, 42, 20}, {30, 43, 19}};

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

const char* get_pin_dir(uint8_t io_pin) {
    static char dir[4] = "";
    memset(dir, 0x00, sizeof(dir));
    uint32_t* out_en_reg = (uint32_t*)DOE31_0_REG_ADDR;
    uint32_t* in_en_reg = (uint32_t*)DIN31_0_REG_ADDR;
    if((1 << io_pin) == ((1 << io_pin) & (*in_en_reg))) {
        snprintf(dir, sizeof(dir), "%si", dir);
    }
    if((1 << io_pin) == ((1 << io_pin) & (*out_en_reg))) {
        snprintf(dir, sizeof(dir), "%so", dir);
    }
    return dir;
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

bool gpio_toggle(uint8_t pin) {
    GPIO_toggle((uint_least8_t)pin);
    return true;
}
