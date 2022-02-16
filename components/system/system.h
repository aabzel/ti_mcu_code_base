#ifndef SYSTEM_H
#define SYSTEM_H

#include <stdbool.h>
#include <stdint.h>

typedef enum eRetx_t{
  RETX_NO_NEED= 0,
  RETX_NEED =1,
}Retx_t;

extern uint32_t send_err_cnt;

#ifdef HAS_IAR
typedef __packed  enum eInterfaces_t {
#else
typedef enum eInterfaces_t {
#endif
    IF_LORA = 0,/*RTCM3 source*/
    IF_SX1262 = 1,
    IF_UART1 = 2, /*for RTCM3 source*/
    IF_LOOPBACK = 3,/*for self Test*/
    IF_CNT = 4,
    IF_CAN = 5,/*for Debug*/
    IF_RS232 = 6,/*for Debug*/
    IF_UART0 = 7,
    IF_BLE = 8,
    IF_SPI0 = 9,
    IF_NONE = 11,
#ifdef HAS_IAR
}  Interfaces_t;
#else
} __attribute__((__packed__)) Interfaces_t;
#endif

bool system_calc_byte_rate(void);
bool sys_send_if(uint8_t* array, uint32_t len, Interfaces_t interface, Retx_t retx);
bool interface_valid(Interfaces_t interface);
const char* interface2str(Interfaces_t interface);
#ifdef HAS_HARVESTER_V1
bool sys_bypass_nmea_rs232(void);
#endif
#endif /* SYSTEM_H  */
