
#include "cli_wrappers.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h> // for sleep function
#include <windows.h>

#include "boot_driver.h"
#include "byte_utils.h"
#include "convert.h"
#include "crc16_ccitt.h"
#include "data_utils.h"
#include "debug_info.h"
#include "fw_update.h"
#include "io_utils.h"
#include "scan_serial_port.h"
#include "str_utils_ex.h"

bool cli_wrp_errase_app(void) {
    bool res = false;
#ifdef HAS_FW_UPDATE_DEBUG
    printf("\n[d]%s():", __FUNCTION__);
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
#ifdef HAS_FW_UPDATE_DEBUG
    printf("\n[d]%s():", __FUNCTION__);
#endif
    char txBuffer[100] = "";
    strncpy(txBuffer, "vi" CRLF, sizeof(txBuffer));
    res = com_send_str(hComm, txBuffer, strlen(txBuffer));
    Sleep(500);
    char rxBuffer[1000] = "";
    uint32_t real_rx_len = 0;
    res = com_receive_str(hComm, rxBuffer, sizeof(rxBuffer), &real_rx_len);
    if(0 < real_rx_len) {
        printf("\n[%s]", rxBuffer);
        res = true;
    }
    return res;
}

bool cli_wrp_restore_target(void) {
    bool res = false;
#ifdef HAS_FW_UPDATE_DEBUG
    printf("\n[d]%s():", __FUNCTION__);
#endif
    char txBuffer[100] = "";
    strncpy(txBuffer, "e 1" CRLF, sizeof(txBuffer));
    res = com_send_str(hComm, txBuffer, strlen(txBuffer));
    Sleep(500);
    char rxBuffer[200] = "";
    uint32_t real_rx_len = 0;
    res = com_receive_str(hComm, rxBuffer, sizeof(rxBuffer), &real_rx_len);
    if(0 < real_rx_len) {
        printf("\n[%s]", rxBuffer);
        printf("\n[d] Target connected!");
        res = true;
    }
    return res;
}

bool is_target_connected(void) {
    bool res = false;
#ifdef HAS_FW_UPDATE_DEBUG
    printf("\n[d]%s():", __FUNCTION__);
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
#ifdef HAS_CONNECTON_DEBUG
        printf("\n[%s] len: %u", rxBuffer, real_rx_len);
#endif
        char* ptr = strstr(rxBuffer, "config:");
        if(ptr) {
            printf("\n[d] Target connected!");
            res = true;
        } else {
            res = false;
        }
    } else {
        res = false;
    }
    return res;
}

bool cli_wrp_flash_write(uint32_t base_addr, uint8_t* chunk_data, uint32_t size) {
    bool res = false;
#ifdef HAS_FW_UPDATE_DEBUG
    printf("\n[d]%s(): size: %u", __FUNCTION__, size);
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
                        printf("\n[e] Target output: [%s] len:%u", rxBuffer, real_rx_len);
                        res = false;
                    }
                } else {
                    printf("\n[e] flash write response len error");
                    res = false;
                }
            } else {
                printf("\n[e] flash write response read error");
                res = false;
            }
        } else {
            printf("\n[e] hex2ascii error");
            res = false;
        }
    }
    return res;
}

bool cli_wrp_flash_read(uint32_t addr, uint8_t* out_array, uint32_t len) {
    bool res = false;
#ifdef HAS_FW_UPDATE_DEBUG
    printf("\n[d]%s(): addr:0x%x len:%u", __FUNCTION__, addr, len);
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
        printf("\n COM:[%s] readLen: %u", rxBuffer, real_rx_len);
#endif
        bool res_data = parse_lhex_array_after_prefix("0x", 0, rxBuffer, out_array, len);
        if(false == res_data) {
            printf("\n[e] parse data error");
        }
#ifdef HAS_FW_UPDATE_DEBUG
        printf("\n");
        print_mem(out_array, len, true, false, true, false);
#endif
        Type16Union_t un16 = {0};
        bool res_crc = parse_lhex_array_after_prefix("0x", 1, rxBuffer, &un16.u8[0], 2);
        if(false == res_crc) {
            printf("\n[e] parse crc16 array error");
        }
        if(res_crc && res_data) {
            crc16_read = reverse_byte_order_uint16(un16.u16);
            crc16_calc = calc_crc16_ccitt_false(out_array, len);
            if(crc16_calc == crc16_read) {
#ifdef HAS_FW_UPDATE_DEBUG
                printf("\n[*] CRC16 Ok");
#endif
                res = true;
            } else {
                printf("\n[e] CRC16 error read: 0x%04x calc: 0x%04x", crc16_read, crc16_calc);
                res = false;
            }
        }
    } else {
        printf("\n[e] Lack of read data error len: %u", real_rx_len);
        printf("\n COM:[%s] readLen: %u", rxBuffer, real_rx_len);
    }
    return res;
}

bool cli_wrp_flash_write_verify(uint32_t base_addr, uint8_t* chunk_data, uint32_t size) {
    bool res = false;
#ifdef HAS_FW_UPDATE_DEBUG
    printf("\n[d]%s(): size: %u", __FUNCTION__, size);
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
                printf("\n[e] flash read data unmatch error");
                res = false;
            }
        } else {
            printf("\n[e] flash read error");
            res = false;
        }
    } else {
        printf("\n[e] flash write error size: %u", size);
    }
    return res;
}
