
#include "cli_wrappers.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>
//#include <unistd.h> // for sleep function clang error
#include <windows.h>

#include "boot_driver.h"
#include "byte_utils.h"
#include "convert.h"
#include "crc16_ccitt.h"
#include "data_utils.h"
#include "debug_info.h"
#include "io_utils.h"
#include "log.h"
#include "scan_serial_port.h"
#include "str_utils.h"
#include "str_utils_ex.h"


#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

void cli_win_color_enable(void) {
#if defined WIN32 || defined _WIN32 || defined WIN64 || defined _WIN64
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);

    // References:
    // SetConsoleMode() and ENABLE_VIRTUAL_TERMINAL_PROCESSING?
    // https://stackoverflow.com/questions/38772468/setconsolemode-and-enable-virtual-terminal-processing

    // Windows console with ANSI colors handling
    // https://superuser.com/questions/413073/windows-console-with-ansi-colors-handling
#endif
}

bool cli_wrp_errase_app(void) {
    bool res = false;
#ifdef HAS_FW_UPDATE_DEBUG
    LOG_DEBUG(SYS, "%s():", __FUNCTION__);
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
    LOG_DEBUG(SYS, "%s():", __FUNCTION__);
#endif
    char txBuffer[100] = "";
    strncpy(txBuffer, "vi" CRLF, sizeof(txBuffer));
    res = com_send_str(hComm, txBuffer, strlen(txBuffer));
    Sleep(500);
    char rxBuffer[1000] = "";
    uint32_t real_rx_len = 0;
    res = com_receive_str(hComm, rxBuffer, sizeof(rxBuffer), &real_rx_len);
    if(0 < real_rx_len) {
        LOG_DEBUG(SYS, "[%s]", rxBuffer);
        res = true;
    }
    return res;
}

bool cli_wrp_restore_target(void) {
    bool res = false;
#ifdef HAS_FW_UPDATE_DEBUG
    LOG_DEBUG(SYS, "%s():", __FUNCTION__);
#endif
    char txBuffer[100] = "";
    strncpy(txBuffer, "e 1" CRLF, sizeof(txBuffer));
    res = com_send_str(hComm, txBuffer, strlen(txBuffer));
    Sleep(500);
    char rxBuffer[200] = "";
    uint32_t real_rx_len = 0;
    res = com_receive_str(hComm, rxBuffer, sizeof(rxBuffer), &real_rx_len);
    if(0 < real_rx_len) {
        LOG_DEBUG(SYS, "[%s]", rxBuffer);
        LOG_DEBUG(SYS, "Target connected!");
        res = true;
    }
    return res;
}

bool is_target_connected(void) {
    bool res = false;
#ifdef HAS_FW_UPDATE_DEBUG
    LOG_DEBUG(SYS, "%s():", __FUNCTION__);
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
        LOG_DEBUG(SYS, "[%s] len: %u", rxBuffer, real_rx_len);
        char* ptr = strstr(rxBuffer, "config:");
        if(ptr) {
            LOG_INFO(SYS, "Target connected!");
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
    LOG_DEBUG(SYS, "%s(): Str:[%s] Len:%u\n", __FUNCTION__, txBuffer, tx_buff_len);
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
            const char* ptr = str_case_str(response, "OK");
            if(ptr) {
                LOG_INFO(SYS, "CmdAck");
                res = true;
            } else {
                LOG_ERROR(SYS, "CmdNak [%s]", response);
                res = false;
            }
        } else {
            res = false;
        }
    }
    return res;
}

bool cli_wrp_flash_write(uint32_t base_addr, uint8_t* chunk_data, uint32_t size) {
    bool res = false;
#ifdef HAS_FW_UPDATE_DEBUG
    LOG_DEBUG(SYS,"%s(): size: %u", __FUNCTION__, size);
#endif
    char txBuffer[1000] = "";
    if(FLASH_WR_CHUNK_SIZE == size) {

        uint8_t HexAscii[2 * FLASH_WR_CHUNK_SIZE + 1];
        memset(HexAscii, 0x00, sizeof(HexAscii));
        res = hex2ascii(chunk_data, size, HexAscii, sizeof(HexAscii));
        if(res) {
            uint16_t crc16_calc = 0;
            crc16_calc = calc_crc16_ccitt_false(chunk_data, size);
            snprintf(txBuffer, sizeof(txBuffer), "fw 0x%08x 0x%s 0x%04x\n\r", base_addr, HexAscii, crc16_calc);
            res = com_send_str(hComm, txBuffer, strlen(txBuffer));
            // Sleep(10*sizeof(HexAscii)); /*Works!*/
            Sleep(5 * sizeof(HexAscii));
            char rxBuffer[1000] = "";
            uint32_t real_rx_len = 0;
            res = com_receive_str(hComm, rxBuffer, sizeof(rxBuffer), &real_rx_len);
            if(res) {
                if(0 < real_rx_len) {
                    if(strstr(rxBuffer, "Ok!")) {
                        res = true;
                    } else {
                        printf("\n[e] flash write status error");
                        LOG_ERROR(SYS,"Target output: [%s] len:%u", rxBuffer, real_rx_len);
                        res = false;
                    }
                } else {
                    LOG_ERROR(SYS,"flash write response len error");
                    res = false;
                }
            } else {
                LOG_ERROR(SYS,"flash write response read error");
                res = false;
            }
        } else {
            LOG_ERROR(SYS,"hex2ascii error");
            res = false;
        }
    }
    return res;
}

bool cli_wrp_flash_read(uint32_t addr, uint8_t* out_array, uint32_t len) {
    bool res = false;
#ifdef HAS_FW_UPDATE_DEBUG
    LOG_DEBUG(SYS,"%s(): addr:0x%x len:%u", __FUNCTION__, addr, len);
#endif
    char txBuffer[1000] = "";
    snprintf(txBuffer, sizeof(txBuffer), "fr 0x%08x %u" CRLF, addr, len);
    res = com_send_str(hComm, txBuffer, strlen(txBuffer));
    // Sleep(3*len*2); /*Works*/
    // Sleep(3*len);/*Works*/
    // Sleep(len);/*Works*/
    char rxBuffer[1000] = "";
    uint32_t real_rx_len = 0;
    uint16_t crc16_read = 0;
    uint16_t crc16_calc = 0;
    res = com_receive_str(hComm, rxBuffer, sizeof(rxBuffer), &real_rx_len);
    if((2 * len) < real_rx_len) {
#ifdef HAS_FW_UPDATE_DEBUG
        LOG_INFO(COM,"[%s] readLen: %u", rxBuffer, real_rx_len);
#endif
        bool res_data = parse_lhex_array_after_prefix("0x", 0, rxBuffer, out_array, len);
        if(false == res_data) {
            LOG_ERROR(SYS,"parse data error");
        }
#ifdef HAS_FW_UPDATE_DEBUG
        print_mem(out_array, len, true, false, true, false);
#endif
        Type16Union_t un16 = {0};
        bool res_crc = parse_lhex_array_after_prefix("0x", 1, rxBuffer, &un16.u8[0], 2);
        if(false == res_crc) {
            LOG_ERROR(SYS,"parse crc16 array error");
        }
        if(res_crc && res_data) {
            crc16_read = reverse_byte_order_uint16(un16.u16);
            crc16_calc = calc_crc16_ccitt_false(out_array, len);
            if(crc16_calc == crc16_read) {
#ifdef HAS_FW_UPDATE_DEBUG
                LOG_INFO(SYS,"CRC16 Ok");
#endif
                res = true;
            } else {
                LOG_ERROR(SYS,"CRC16 error read: 0x%04x calc: 0x%04x", crc16_read, crc16_calc);
                res = false;
            }
        }
    } else {
        LOG_ERROR(SYS,"Lack of read data error len: %u", real_rx_len);
        LOG_INFO(COM,"[%s] readLen: %u", rxBuffer, real_rx_len);
    }
    return res;
}


bool cli_wrp_flash_write_verify(uint32_t base_addr, uint8_t* chunk_data, uint32_t size) {
    bool res = false;
#ifdef HAS_FW_UPDATE_DEBUG
    LOG_DEBUG(SYS,"%s(): size: %u", __FUNCTION__, size);
#endif
    res = cli_wrp_flash_write(base_addr, chunk_data, size);
    if(res && (FLASH_WR_CHUNK_SIZE == size)) {
        uint8_t ReadChunk[FLASH_WR_CHUNK_SIZE];
        memset(ReadChunk, 0xFF, sizeof(ReadChunk));
        res = cli_wrp_flash_read(base_addr, ReadChunk, sizeof(ReadChunk));
        if(res) {
            int ret = memcmp(ReadChunk, chunk_data, size);
            if(0 == ret) {
                res = true;
            } else {
                LOG_ERROR(SYS,"flash read data unmatch error");
                res = false;
            }
        } else {
            LOG_ERROR(SYS,"flash read error");
            res = false;
        }
    } else {
        LOG_ERROR(SYS,"flash write error size: %u", size);
    }
    return res;
}
