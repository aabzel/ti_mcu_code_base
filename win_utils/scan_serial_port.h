#ifndef SCAN_SERIAL_PORT_H
#define SCAN_SERIAL_PORT_H

// https://www.xanthium.in/Serial-Port-Programming-using-Win32-API

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include <windows.h>

#define DEDUG_RX_TEXT 0
#define MAX_COM_NUM 20
#define DEBUG_FINE_VI_REQ 0
#define DEBUG_SPOT_COM 0
#define DEBUG_FAILED_OPENS 0

#define MAX_NUM_COM_DEV 40U

typedef enum { UNDEF_DEVICE = 0 } deciceId_t;

typedef struct xConnection_t {
    char comPortName[30];
    char deviceName[30];
    bool isExistPort;
    bool isExistDevice;
    uint64_t serialNumber;
    uint16_t deviceID;
} xSerialConnection_t;

extern xSerialConnection_t deviceList[MAX_COM_NUM];
extern HANDLE hComm;

bool init_serial(char* com_name, uint16_t baud_rate);
bool is_serial_known(uint64_t inSerial);
const char* dev_id_name(deciceId_t deviceID);
bool scan_serial(void);
bool print_device_list(void);
bool com_send_str(HANDLE hComm, char* txBuffer, uint32_t txBuffLen);
uint16_t parse_product(char* inStr, uint16_t inStrLen);
char* parse_product_name(char* inStr, uint16_t inStrLen);
bool com_receive_str(HANDLE hComm, char* outRxArray, uint32_t capasityRxArray, uint32_t* outRealRxArrayLen);

#ifdef __cplusplus
}
#endif

#endif /* SCAN_SERIAL_PORT_H */