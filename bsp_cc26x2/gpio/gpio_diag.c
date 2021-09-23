#include "gpio_diag.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/gpio/GPIOCC26XX.h>

#include "bit_utils.h"
#include "gpio_drv.h"

const char* get_gpio_mode(uint8_t pin) {
    char* name = "_";
    GPIO_PinConfig pin_cfg = 0;
    if(pin < GPIO_COUNT) {
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
        name = "I2C_MSDA";
        break;
    case 0xE:
        name = "I2C_MSCL";
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

const char* get_pin_dir(uint8_t dioNumber) {
    uint32_t out_en = GPIO_getOutputEnableDio((uint32_t)dioNumber);
    static char dir[4] = "";
    memset(dir, 0x00, sizeof(dir));
    if(GPIO_OUTPUT_ENABLE == out_en) {
        snprintf(dir, sizeof(dir), "%so", dir);
    } else {
        snprintf(dir, sizeof(dir), "%si", dir);
    }
    return dir;
}
