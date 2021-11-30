#include "system.h"

#include <stdbool.h>

#include "uart_common.h"
#include "sys_config.h"
#include "io_utils.h"

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

static bool call_recursion(uint32_t cur_depth,
                           uint32_t max_depth,
                           uint32_t* stack_size){
    bool res = false;
    if (cur_depth < max_depth) {
        res = call_recursion(cur_depth+1, max_depth, stack_size);
    } else if ( cur_depth == max_depth) {
        uint32_t top_stack_val = *((uint32_t*)(APP_START_ADDRESS));
        uint32_t cur_stack_use=  top_stack_val-((uint32_t)&res);
        *stack_size=cur_stack_use;
        res = true;
    }else{
        res = false;
    }
    return res;
}

bool try_recursion(uint32_t max_depth, uint32_t* stack_size){
    bool res = false;
    res = call_recursion(0,max_depth, stack_size);
    return res;
}
