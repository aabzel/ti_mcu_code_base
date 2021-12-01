#ifndef SYSTEM_H
#define SYSTEM_H

#include <stdbool.h>
#include <stdint.h>

typedef enum eInterfaces_t {
    IF_LORA=0,
    IF_RS232=1,
    IF_UART1=2,
    IF_CAN=3,
    IF_BLE=4,
    IF_NONE=5,
} __attribute__ ((__packed__)) Interfaces_t;

const char* interface2str(Interfaces_t interface);
bool sys_bypass_nmea_rs232(void);

#endif /* SYSTEM_H  */
