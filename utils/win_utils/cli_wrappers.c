
#include "cli_wrappers.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>
//#include <unistd.h> // for sleep function clang error
#include <windows.h>

#include "log.h"
#include "boot_driver.h"
#include "byte_utils.h"
#include "convert.h"
#include "crc16_ccitt.h"
#include "data_utils.h"
#include "debug_info.h"
#include "io_utils.h"
#include "scan_serial_port.h"
#include "str_utils.h"
#include "str_utils_ex.h"

#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

void cli_win_color_enable(void){
#if defined WIN32 || defined _WIN32 || defined WIN64 || defined _WIN64
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);

    // References:
    //SetConsoleMode() and ENABLE_VIRTUAL_TERMINAL_PROCESSING?
    //https://stackoverflow.com/questions/38772468/setconsolemode-and-enable-virtual-terminal-processing

    // Windows console with ANSI colors handling
    // https://superuser.com/questions/413073/windows-console-with-ansi-colors-handling
#endif

}

bool cli_wrp_errase_app(void) {
    bool res = false;
#ifdef HAS_FW_UPDATE_DEBUG
    LOG_DEBUG(SYS,"%s():", __FUNCTION__);
#endif
    char txBuffer[100] = "";
    snprintf(txBuffer, sizeof(txBuffer), "ea" CRLF);
    res = com_send_str(hComm, txBuffer, strlen(txBuffer));
    Sleep(2000);
    char rxBuffer[1000] = "";
    uint32_t real_rx_len = 0;
    res = com_receive_str(hComm, rxBuffer, sizeof(rxBuffer), &real_rx_len);
    return res;
}

bool cli_wrp_parse_fw_version(void) {
    bool res = false;
#ifdef HAS_DEBUG
    LOG_DEBUG(SYS,"%s():", __FUNCTION__);
#endif
    char txBuffer[100] = "";
    strncpy(txBuffer, "vi" CRLF, sizeof(txBuffer));
    res = com_send_str(hComm, txBuffer, strlen(txBuffer));
    Sleep(500);
    char rxBuffer[1000] = "";
    uint32_t real_rx_len = 0;
    res = com_receive_str(hComm, rxBuffer, sizeof(rxBuffer), &real_rx_len);
    if(0 < real_rx_len) {
        LOG_DEBUG(SYS,"[%s]", rxBuffer);
        res = true;
    }
    return res;
}

bool cli_wrp_restore_target(void) {
    bool res = false;
#ifdef HAS_FW_UPDATE_DEBUG
    LOG_DEBUG(SYS,"%s():", __FUNCTION__);
#endif
    char txBuffer[100] = "";
    strncpy(txBuffer, "e 1" CRLF, sizeof(txBuffer));
    res = com_send_str(hComm, txBuffer, strlen(txBuffer));
    Sleep(500);
    char rxBuffer[200] = "";
    uint32_t real_rx_len = 0;
    res = com_receive_str(hComm, rxBuffer, sizeof(rxBuffer), &real_rx_len);
    if(0 < real_rx_len) {
        LOG_DEBUG(SYS,"[%s]", rxBuffer);
        LOG_DEBUG(SYS,"Target connected!");
        res = true;
    }
    return res;
}

bool is_target_connected(void) {
    bool res = false;
#ifdef HAS_FW_UPDATE_DEBUG
    LOG_DEBUG(SYS,"%s():", __FUNCTION__);
#endif
    char txBuffer[100] = "";
    strncpy(txBuffer, "e 0" CRLF, sizeof(txBuffer));
    res = com_send_str(hComm, txBuffer, strlen(txBuffer));
    Sleep(100);
    strncpy(txBuffer, "vi" CRLF, sizeof(txBuffer));
    res = com_send_str(hComm, txBuffer, strlen(txBuffer));

    Sleep(100);
    char rxBuffer[200] = "";
    uint32_t real_rx_len = 0;
    res = com_receive_str(hComm, rxBuffer, sizeof(rxBuffer), &real_rx_len);
    if(0 < real_rx_len) {
        LOG_DEBUG(SYS,"[%s] len: %u", rxBuffer, real_rx_len);
        char* ptr = strstr(rxBuffer, "config:");
        if(ptr) {
            LOG_INFO(SYS,"Target connected!");
            res = true;
        } else {
            res = false;
        }
    } else {
        res = false;
    }
    return res;
}

bool cli_cmd_send(HANDLE hComm, char* txBuffer, uint32_t tx_buff_len) {
    bool res = false;
    LOG_DEBUG(SYS,"%s(): Str:[%s] Len:%u\n", __FUNCTION__, txBuffer, tx_buff_len);
    char Buffer[1000] = "";
    snprintf(Buffer, sizeof(Buffer), "%s" CRLF, txBuffer);
    uint32_t cmd_len = strlen(Buffer);
    res = com_send_str(hComm, Buffer, cmd_len);
    char response[1000];
    uint32_t out_len = 0;
    if(res) {
        res = com_receive_str_timeout(hComm, response, sizeof(response), &out_len, 500);
    }
    if(res) {
        if(0 < out_len) {
            char* ptr = str_case_str(response, "OK");
            if(ptr) {
                LOG_INFO(SYS,"CmdAck");
                res = true;
            } else {
                LOG_ERROR(SYS,"CmdNak [%s]", response);
                res = false;
            }
        } else {
            res = false;
        }
    }
    return res;
}
