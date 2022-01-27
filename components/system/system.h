#ifndef SYSTEM_H
#define SYSTEM_H

#include <stdbool.h>
#include <stdint.h>

#ifdef HAS_IAR
typedef __packed  enum eInterfaces_t {
#else
typedef enum eInterfaces_t {
#endif
    IF_LORA = 0,/*RTCM3 sourse*/
    IF_CAN = 1,/*for Debug*/
    IF_RS232 = 2,/*for Debug*/
    IF_LOOPBACK = 3,/*for self Test*/
    IF_UART1 = 4, /*for RTCM3 sourse*/
    IF_CNT = 5,
    IF_UART0 = 6,
    IF_BLE = 7,
    IF_SPI0 = 8,
    IF_NONE = 9,
#ifdef HAS_IAR
}  Interfaces_t;
#else
} __attribute__((__packed__)) Interfaces_t;
#endif

bool sys_send_if(uint8_t* array, uint32_t len, Interfaces_t interface);
bool interface_valid(Interfaces_t interface);
const char* interface2str(Interfaces_t interface);
#ifdef HAS_HARVESTER_V1
bool sys_bypass_nmea_rs232(void);
#endif
#endif /* SYSTEM_H  */
