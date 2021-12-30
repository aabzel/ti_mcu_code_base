#include "scan_serial_port.h"

#include <stdio.h>
#include <time.h>
#include <sys/time.h>

#include "log.h"
#include "convert.h"
#include "str_utils.h"
#include "win_utils.h"

#ifdef HAS_MCU
#error That code only for desktop builds
#endif

HANDLE hComm;

xSerialConnection_t deviceList[MAX_COM_NUM];

static bool com_set_params(HANDLE hComm, uint32_t baud_rate) {
    bool res = false;
    // Initializing DCB structure
    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    BOOL status = GetCommState(hComm, &dcbSerialParams);
    dcbSerialParams.BaudRate = baud_rate;  // Setting BaudRate = 460800
    dcbSerialParams.ByteSize = 8;          // Setting ByteSize = 8
    dcbSerialParams.StopBits = ONESTOPBIT; // Setting StopBits = 1
    dcbSerialParams.Parity = NOPARITY;     // Setting Parity = None

    SetCommState(hComm, &dcbSerialParams);
    if(status) {
        res = true;
    }
    return res;
}

static bool com_set_timeout(HANDLE hComm) {
    bool res = false;
    COMMTIMEOUTS timeouts = {0};
    timeouts.ReadIntervalTimeout = 2;        // in milliseconds
    timeouts.ReadTotalTimeoutMultiplier = 5; //
    timeouts.ReadTotalTimeoutConstant = 100; // in milliseconds
    timeouts.WriteTotalTimeoutConstant = 1;  // in milliseconds
    timeouts.WriteTotalTimeoutMultiplier = 1;

    SetCommTimeouts(hComm, &timeouts);
    return res;
}

bool com_send_str(HANDLE hComm, char* txBuffer, uint32_t tx_buff_len) {
#ifdef HAS_COM_PORT_DEBUG
    LOG_DEBUG(COM,"%s() send [%s]", __FUNCTION__, txBuffer);
#endif
    bool res = false;
    uint32_t remain_len=0;
    res = com_receive_remain(hComm, &remain_len);
    BOOL status;
    DWORD dNoOfBytesWritten = 0;
    status = WriteFile(hComm, txBuffer, (DWORD)tx_buff_len, &dNoOfBytesWritten, NULL);
    if(dNoOfBytesWritten == tx_buff_len) {
        if(status) {
            res = true;
        }
    }
#ifdef HAS_COM_PORT_DEBUG
    LOG_DEBUG(COM,"%s() end", __FUNCTION__);
#endif
    return res;
}

bool com_receive_remain(HANDLE hComm, uint32_t* remain_len) {
    bool res = false;
#ifdef HAS_COM_PORT_DEBUG
    LOG_DEBUG(COM,"%s():", __FUNCTION__);
#endif
    if(NULL != remain_len) {
        *remain_len = 0;
        char tempChar;
        DWORD numberBytesRead;
        uint32_t BytesReadCnt = 0;
        bool loopRun = true;
        while(loopRun) {
            ReadFile(hComm,            // Handle of the Serial port
                     &tempChar,        // Temporary character
                     sizeof(tempChar), // Size of TempChar
                     &numberBytesRead, // Number of bytes read
                     NULL);
#if DEDUG_RX_CHAR
            LOG_DEBUG(COM,"%c", tempChar);
#endif
            if(0 < numberBytesRead) {
                loopRun = true;
            } else {
                loopRun = false;
            }
            BytesReadCnt++;
        };
        if(0 < BytesReadCnt) {
            *remain_len = BytesReadCnt;
            res = true;
        }
    }
    return res;
}

bool com_receive_str(HANDLE hComm, char* outRxArray, uint32_t capasityRxArray, uint32_t* remain_len) {
    bool res = false;
#ifdef HAS_COM_PORT_DEBUG
    LOG_DEBUG(COM,"%s():", __FUNCTION__);
#endif
    if((0 < capasityRxArray) && (NULL != outRxArray)) {
        *remain_len = 0;
        char tempChar;
        DWORD numberBytesRead;
        uint32_t BytesReadCnt = 0;
        bool loopRun = true;
        uint32_t  ret= 0;
        while(loopRun) {
            ret = ReadFile(hComm,            // Handle of the Serial port
                     &tempChar,        // Temporary character
                     sizeof(tempChar), // Size of TempChar
                     &numberBytesRead, // Number of bytes read
                     NULL);
            if(0==ret){
                loopRun = false;
            }
#ifdef DEDUG_RX_CHAR
            LOG_DEBUG(COM,"%c", tempChar);
#endif
            if(0 < numberBytesRead) {
                if(BytesReadCnt < capasityRxArray) {
                    outRxArray[BytesReadCnt] = tempChar; // Store Tempchar into buffer
                }
            } else {
                loopRun = false;
            }
            BytesReadCnt++;
        };
        if(0 < BytesReadCnt) {
            *remain_len = BytesReadCnt;
            res = true;
        }
    }
#ifdef HAS_COM_PORT_DEBUG
    LOG_DEBUG(COM,"%s(): end", __FUNCTION__);
#endif
    return res;
}




bool com_receive_str_timeout(HANDLE hComm, char* outRxArray, uint32_t capasityRxArray, uint32_t* remain_len, uint32_t time_out_ms) {
    bool res = false;
#ifdef HAS_COM_PORT_DEBUG
    LOG_DEBUG(COM,"%s():", __FUNCTION__);
#endif
    if((0 < capasityRxArray) && (NULL != outRxArray)) {
        *remain_len = 0;
        char tempChar;
        DWORD numberBytesRead =0;
        uint32_t BytesReadCnt = 0;
        bool loopRun = true;
        uint32_t  ret= 0;
        struct timeval start, cur;
        double diff_secs = 0;
        gettimeofday(&start, NULL);
        while(loopRun) {
            gettimeofday(&cur, NULL);
            diff_secs = (double)(cur.tv_usec - start.tv_usec) / 1000000 + (double)(cur.tv_sec - start.tv_sec);
            if(time_out_ms<diff_secs*1000 ){
                loopRun = false;
            }
            ret = ReadFile(hComm,            // Handle of the Serial port
                     &tempChar,        // Temporary character
                     sizeof(tempChar), // Size of TempChar
                     &numberBytesRead, // Number of bytes read
                     NULL);
            if(0==ret){

            }
#ifdef DEDUG_RX_CHAR
            LOG_DEBUG(COM,"%c", tempChar);
#endif
            if(0 < numberBytesRead) {
                if(BytesReadCnt < capasityRxArray) {
                    outRxArray[BytesReadCnt] = tempChar; // Store Tempchar into buffer
                }
            } else {
                loopRun = false;
            }
            BytesReadCnt++;
        };
        if(0 < BytesReadCnt) {
            *remain_len = BytesReadCnt;
            res = true;
        }
    }
#ifdef HAS_COM_PORT_DEBUG
    LOG_DEBUG(COM,"%s(): end", __FUNCTION__);
#endif
    return res;
}


static uint64_t knownSerialTable[MAX_NUM_COM_DEV] = {0};

bool is_serial_known(uint64_t inSerial) {
    int i;
    for(i = 0; i < MAX_NUM_COM_DEV; i++) {
        if(inSerial == knownSerialTable[i]) {
            return true;
        }
    }
    for(i = 0; i < MAX_NUM_COM_DEV; i++) {
        if(0 == knownSerialTable[i]) {
            knownSerialTable[i] = inSerial;
            return false;
        }
    }
    return false;
}

static bool print_new_dev_in_file(char* file_name, uint8_t comPortNum, uint64_t serialNumber, char* deviceName) {
    FILE* client_log_p;
    bool res = false;
    client_log_p = fopen(file_name, "a");
    if(NULL != client_log_p) {
        fclose(client_log_p);
        res = true;
    } else {
        LOG_ERROR(COM,"Unable to open file");
    }
    return res;
}

bool init_serial(char* com_name, uint32_t baud_rate) {
    bool res = false;

#if DEBUG_SERIAL
    LOG_DEBUG(COM,"try open [%s]...", com_name);
#endif

    hComm = CreateFile(com_name,
                       GENERIC_READ | GENERIC_WRITE, // Read/Write
                       0,                            // No Sharing
                       NULL,                         // No Security
                       OPEN_EXISTING,                // Open existing port only
                       0,                            // Non Overlapped I/O
                       NULL);                        // Null for Comm Devices
#if DEBUG_SERIAL
    LOG_DEBUG(COM,"\n[i] hComm [%p]", hComm);
#endif
    if(INVALID_HANDLE_VALUE == hComm) {
#if DEBUG_SERIAL
        LOG_ERROR(COM,"Unable to open serial port [%s]\n", com_name);
#endif
        res = false;
    } else {
#if DEBUG_SERIAL
        LOG_DEBUG(COM,"\n[i] Open [%s] OK\n", com_name);
#endif
        res = true;

        com_set_params(hComm, baud_rate);
        com_set_timeout(hComm);
    }

    return res;
}

bool scan_serial(void) {
    bool res = false;
    clear_tui();
    LOG_INFO(COM,"Start new scan");
    bool out_res = false;
    char com_name[20] = "";
    uint8_t comPortNum;
    for(comPortNum = 0; comPortNum <= MAX_COM_NUM; comPortNum++) {
        snprintf(com_name, sizeof(com_name), "COM%u", comPortNum);
#if DEBUG_FAILED_OPENS
        LOG_DEBUG(COM,"\n try [%s]...", com_name);
#endif
        HANDLE hComm;
#if 1
        hComm = CreateFile(com_name,
                           GENERIC_READ | GENERIC_WRITE, // Read/Write
                           0,                            // No Sharing
                           NULL,                         // No Security
                           OPEN_EXISTING,                // Open existing port only
                           0,                            // Non Overlapped I/O
                           NULL);                        // Null for Comm Devices
#if DEBUG_FAILED_OPENS
        LOG_DEBUG(COM,"\n hComm [%p]", hComm);
#endif
        if(hComm == INVALID_HANDLE_VALUE) {
#if DEBUG_FAILED_OPENS
            LOG_DEBUG(COM,"\nUnable open serial port [%s]", com_name);
#endif
        } else {
#if DEBUG_SPOT_COM
            LOG_DEBUG(COM,"\n [%s] exists in PC", com_name);
#endif
            deviceList[comPortNum].isExistPort = true;
            CloseHandle(hComm);
            out_res = true;
        }
#endif

        snprintf(com_name, sizeof(com_name), "\\\\.\\COM%u", comPortNum);
        hComm = CreateFile(com_name, GENERIC_READ | GENERIC_WRITE, 0, // No Sharing
                           NULL,                                      // No Security
                           OPEN_EXISTING,
                           0,     // Non Overlapped I/O
                           NULL); // Null for Comm Devices
#if DEBUG_FAILED_OPENS
        LOG_DEBUG(COM,"\n hComm [%p]", hComm);
#endif
        if(hComm == INVALID_HANDLE_VALUE) {
#if DEBUG_FAILED_OPENS
            LOG_DEBUG(COM,"\n  Error in opening serial port [%s]", com_name);
#endif
        } else {
#if DEBUG_SPOT_COM
            LOG_DEBUG(COM,"\n [%s] exists in PC", com_name);
#endif
            deviceList[comPortNum].isExistPort = true;
            com_set_params(hComm, 9600);
            com_set_timeout(hComm);
            uint32_t realRxLen = 0;

            res = com_receive_remain(hComm, &realRxLen);

            char txBuffer[] = "vi\n\r";
            res = com_send_str(hComm, txBuffer, strlen(txBuffer));
            if(true == res) {
#if DEBUG_FINE_VI_REQ
                LOG_DEBUG(COM,"\n vi request ok!");
#endif
            } else {
                LOG_DEBUG(COM,"\n vi request fail!");
            }
            char rxBuffer[10000] = "";
            LOG_DEBUG(COM,"\n");
            res = com_receive_str(hComm, rxBuffer, sizeof(rxBuffer), &realRxLen);
            if(true == res) {
                if(10 < realRxLen) {
#if DEDUG_RX_TEXT
                    LOG_DEBUG(COM,"[%s]\n rx %u bytes", rxBuffer, realRxLen);
#endif

                } else {
                    LOG_DEBUG(COM,"\nLack of response");
                }
            } else {
                LOG_DEBUG(COM,"\n read fail!");
            }

            CloseHandle(hComm);
        }
    }

    print_device_list();
    LOG_DEBUG(COM,"\nScan done\n");

    return out_res;
}

bool print_device_list(void) {
    bool out_res = false;
    bool res = false;
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    LOG_DEBUG(COM,"Now: %d-%02d-%02d %02d:%02d:%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min,
           tm.tm_sec);

    LOG_DEBUG(COM,"Compile Date %s %s ", __DATE__, __TIME__);
    LOG_DEBUG(COM,"local  mac: ");
    uint16_t txTextLen;
    uint32_t comPortNum = 0;

    for(comPortNum = 0; comPortNum < MAX_COM_NUM; comPortNum++) {
        if(true == deviceList[comPortNum].isExistPort) {
            LOG_DEBUG(COM,"\n COM%u ", comPortNum);
            if(true == deviceList[comPortNum].isExistDevice) {
                LOG_DEBUG(COM,"Serial [0x%016llx] Device [%s] ", (long long unsigned int)deviceList[comPortNum].serialNumber,
                       deviceList[comPortNum].deviceName);
                out_res = true;
            } else {
                LOG_DEBUG(COM," lack device");
            }
        }
    }
    LOG_DEBUG(COM,"serial ");
    LOG_DEBUG(COM,"scan ");
    LOG_DEBUG(COM,"done ");

    for(comPortNum = 0; comPortNum < MAX_COM_NUM; comPortNum++) {
        if(true == deviceList[comPortNum].isExistPort) {
            if(true == deviceList[comPortNum].isExistDevice) {
                if(false == res) {
                    LOG_DEBUG(COM," Unable to send to TCP server");
                }
            }
        }
    }
    Sleep(4000);
    return out_res;
}

/**/
char* parse_product_name(char* inStr, uint16_t inStrLen) {
    static char devName[50] = "";
    memset(devName, 0x00, sizeof(devName));
    int i, cnt = 0;
    // LOG_DEBUG(COM,"\n%s: in_str [%s] inStrLen %u", __FUNCTION__,inStr, inStrLen );
    uint16_t line_cnt = 0;
    for(i = 0; i < inStrLen; i++) {
        if('\n' == inStr[i]) {
            if(0 == line_cnt) {
                cnt = 0;
            }
        }
        if(' ' != inStr[i]) {
            if(('\n' != inStr[i]) && ('\r' != inStr[i])) {
                devName[cnt] = inStr[i];
                cnt++;
            }
        } else {
            break;
        }
    }

    return devName;
}
