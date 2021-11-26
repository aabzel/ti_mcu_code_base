
#include "system.h"


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

