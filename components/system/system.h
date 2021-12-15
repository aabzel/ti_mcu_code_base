#ifndef SYSTEM_H
#define SYSTEM_H

#include <stdbool.h>
#include <stdint.h>

#ifdef HAS_IAR
typedef __packed  enum eInterfaces_t {
#else
typedef   enum eInterfaces_t {
#endif
    IF_LOOPBACK = 0,
    IF_LORA = 1,
    IF_RS232 = 2,
    IF_UART1 = 3,
    IF_CAN = 4,
    IF_BLE = 5,
    IF_SPI = 6,
    IF_NONE = 7,
#ifdef HAS_IAR
}  Interfaces_t;
#else
} __attribute__((__packed__)) Interfaces_t;
#endif

const char* interface2str(Interfaces_t interface);
#ifdef HAS_HARVESTER_V1
bool sys_bypass_nmea_rs232(void);
#endif
#endif /* SYSTEM_H  */
