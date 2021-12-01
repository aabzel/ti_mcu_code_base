#include "system.h"

#include <stdbool.h>

#include "uart_common.h"
#include "sys_config.h"
#include "io_utils.h"

#ifdef HAS_BOOTLOADER
#error "That API only for Generic"
#endif

const char* interface2str(Interfaces_t interface){
    const char *name="undef";
	switch(interface){
		case IF_LORA:name="LoRa"; break;
		case IF_RS232: name="Rs232"; break;
		case IF_UART1: name="UART1"; break;
		case IF_CAN: name="CAN"; break;
		case IF_BLE: name="BLE"; break;

		default:break;
	}
	return name;
}

bool sys_bypass_nmea_rs232(void) {
    bool res = true;
    huart[UART_NUM_ZED_F9P].is_uart_fwd[UART_NUM_CLI] = true;
    return res;
}

