#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#include "convert.h"
#include "io_utils.h"
#include "scan_serial_port.h"
#include "utils_file.h"

static bool is_target_connected(void) {
    bool res = false;
    char txBuffer[100] = "";
    strncpy(txBuffer, "e 0" CRLF, sizeof(txBuffer));
    res = com_send_str(hComm, txBuffer, strlen(txBuffer));
    sleep(2);
    char rxBuffer[200] = "";
    uint32_t RealRxLen = 0;
    res = com_receive_str(hComm, rxBuffer, sizeof(rxBuffer), &RealRxLen);
    if(0 < RealRxLen) {
        printf("\n[%s]", rxBuffer);
        printf("\n[d] Target connected!");
        res = true;
    }
    return res;
}

static bool restore_target(void){
    bool res = false;
    char txBuffer[100] = "";
    strncpy(txBuffer, "e 1" CRLF, sizeof(txBuffer));
    res = com_send_str(hComm, txBuffer, strlen(txBuffer));
    sleep(2);
    char rxBuffer[200] = "";
    uint32_t RealRxLen = 0;
    res = com_receive_str(hComm, rxBuffer, sizeof(rxBuffer), &RealRxLen);
    if(0 < RealRxLen) {
        printf("\n[%s]", rxBuffer);
        printf("\n[d] Target connected!");
        res = true;
    }
    return res;
}

static bool parse_fw_version(void){
    bool res = false;
    char txBuffer[100] = "";
    strncpy(txBuffer, "vi" CRLF, sizeof(txBuffer));
    res = com_send_str(hComm, txBuffer, strlen(txBuffer));
    sleep(3);
    char rxBuffer[600] = "";
    uint32_t RealRxLen = 0;
    res = com_receive_str(hComm, rxBuffer, sizeof(rxBuffer), &RealRxLen);
    if(0 < RealRxLen) {
        printf("\n[%s]", rxBuffer);
        res = true;
    }
    return res;
}

int main(int argc, char* argv[]) {
    int ret = 0;
    bool res = false;
    printf("Loader!\n");
    if(argc < 3) {
        printf("Lack of firmware");
        ret = 1;
        res = false;
    } else {
        res = true;
    }

    uint16_t baud_rate = 0;
    if(res) {
        res = try_str2uint16(argv[2], &baud_rate);
        if(false == res) {
            printf("\nUnable to extract baud_rate %s", argv[2]);
        }
    }

    if(res) {
        res = init_serial(argv[1], baud_rate);
    }

    if(res) {
        res = is_target_connected();
    }

    if(res) {
        res = parse_fw_version();

    }

    if(res) {
        uint8_t* pFirmwareArray = NULL;
        uint32_t firmware_size = 0;
        res = file_load_to_array(argv[3], &pFirmwareArray, &firmware_size);
    }

    if(res) {
        res = restore_target();
        if(false == res) {
            printf("\n[e] Unable to restore target");
        }
    }

    CloseHandle(hComm);
    return ret;
}
