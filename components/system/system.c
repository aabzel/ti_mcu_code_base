#include "system.h"

#include <stdbool.h>

#ifdef HAS_LORA
#include "lora_drv.h"
#endif

#ifdef HAS_CAN
#include "can_drv.h"
#endif

#ifdef HAS_RS232
#include "rs232_drv.h"
#endif

#ifdef HAS_SPI
#include "spi_drv.h"
#endif

#ifdef HAS_RTCM3
#include "rtcm3_protocol.h"
#endif

#ifdef HAS_LOG
#include "io_utils.h"
#include "log.h"
#endif
#ifdef HAS_MCU
#include "sys_config.h"
#endif

#ifdef HAS_UART
#include "uart_common.h"
#endif

#ifdef HAS_TBFP
#include "tbfp_protocol.h"
#include "tbfp_re_tx_ack_fsm.h"
#endif

#ifdef HAS_SX1262
#include "sx1262_drv.h"
//#include "sx1262_re_tx.h"
#endif

#ifdef HAS_BOOTLOADER
#error "That API only for Generic"
#endif

uint32_t send_err_cnt = 0;

const char* interface2str(Interfaces_t interface) {
    const char* name = "???";
    switch(interface) {
    case IF_SX1262:
        name = "SX1262";
        break;
    case IF_LORA:
        name = "LoRa";
        break;
    case IF_RS232:
        name = "Rs232";
        break;
    case IF_UART1:
        name = "UART1";
        break;
    case IF_UART0:
        name = "UART0";
        break;
    case IF_CAN:
        name = "CAN";
        break;
    case IF_BLE:
        name = "BLE";
        break;
    case IF_LOOPBACK:
        name = "LoopBack";
        break;
    case IF_SPI0:
        name = "SPI0";
        break;
    default:
        break;
    }
    return name;
}


bool system_calc_byte_rate(void){
    rtcm3_calc_byte_rate();
    uart_calc_byte_rate();
    return false;
}
bool interface_valid(Interfaces_t interface) {
    bool res = false;
    switch(interface) {
    case IF_LORA:
    case IF_RS232:
    case IF_UART1:
    case IF_SX1262:
    case IF_SPI0:
    case IF_UART0:
    case IF_CAN:
    case IF_BLE:
        res = true;
        break;
    default:
        res = false;
        break;
    }
    return res;
}

static bool sys_sent_sx1262(uint8_t* array, uint32_t len, Retx_t retx){
    bool res = false;
    switch(retx){
    case RETX_NEED:
        res = tbfp_retx_start(&TbfpProtocol[IF_SX1262], array, len);
        break;
    case RETX_NO_NEED:
        res = sx1262_start_tx(array, len, TX_SINGLE_MODE);
        break;
    default:
        res = false;
        break;
    }
    return res;
}

bool sys_send_if(uint8_t* array, uint32_t len, Interfaces_t interface, Retx_t retx) {
    bool res = false;
    LOG_DEBUG(SYS, "%s Send", interface2str(interface));
    switch(interface) {
#if defined(HAS_SX1262) ||  defined(X86_64)
    case IF_SX1262: {
#ifdef HAS_MCU
        res = sys_sent_sx1262( array,  len,  retx);
#else
        /*ForUnitTest on PC*/
        res = tbfp_proc(&array[0], len, IF_SX1262, true);
#endif /*HAS_MCU*/
    } break;
#endif /*HAS_SX1262 or X86_64*/
#ifdef HAS_LORA
    case IF_LORA: {
        res = lora_send_queue(array, len);
    } break;
#endif
#ifdef HAS_CAN
    case IF_CAN: {
        res = can_send(array, len);
    } break;
#endif
#ifdef HAS_SPI0
    case IF_SPI0: {
        res = spi_write(SPI0_INX, array, len);
    } break;
#endif
#ifdef HAS_UART0
    case IF_UART0: {
        res = uart_send(0, array, len, true);
    } break;
#endif
#ifdef HAS_UART1
    case IF_UART1: {
        res = uart_send(1, array, len, true);
    } break;
#endif
#ifdef HAS_RS232
    case IF_RS232: {
        res = rs232_send(array, len);
    } break;
#endif
#ifdef HAS_BLE
    case IF_BLE: {
        res = ble_send(array, len);
    } break;
#endif
    case IF_LOOPBACK: {
#ifdef HAS_TBFP
        res = tbfp_proc(array, len, IF_LOOPBACK, true);
        if(false == res) {
            LOG_ERROR(SYS, "tbfpProcErr");
        }
#endif
    } break;
    default: {
        LOG_ERROR(SYS, "UndefIf: %u=%s", interface, interface2str(interface));
        res = false;
    } break;
    }
    if(false == res) {
        LOG_DEBUG(SYS, "SendIfErr: %u=%s", interface, interface2str(interface));
        send_err_cnt++;
    }
    return res;
}

#if defined(HAS_HARVESTER_V1) && defined(UART_NUM_ZED_F9P) && defined(HAS_UART)
bool sys_bypass_nmea_rs232(void) {
    bool res = true;
    huart[UART_NUM_ZED_F9P].is_uart_fwd[UART_NUM_CLI] = true;
    return res;
}
#endif
