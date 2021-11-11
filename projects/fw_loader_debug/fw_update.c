#include "fw_update.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h> // for sleep function

#include "boot_driver.h"
#include "byte_utils.h"
#include "cli_wrappers.h"
#include "convert.h"
#include "crc16_ccitt.h"
#include "data_utils.h"
#include "debug_info.h"
#include "io_utils.h"
#include "scan_serial_port.h"
#include "str_utils_ex.h"

bool read_current_firmware(uint8_t** out_firmware, uint32_t* fw_len) {
    bool res = false;
#ifdef HAS_FW_UPDATE_DEBUG
    printf("\n%s():", __FUNCTION__);
#endif
    uint32_t addr = DFLT_APP_START_ADDR;
    uint8_t* pFirmwareFromFlash = (uint8_t*)malloc(MAX_GENERIC_SIZE);
    if(pFirmwareFromFlash) {
        uint8_t try_num = 0;
        uint32_t chunk_num = 0;
        printf("\n[i] Load firmware from flash to RAM...\n");
        for(addr = DFLT_APP_START_ADDR, chunk_num = 0; addr <= (DFLT_APP_END_ADDR - FLASH_RD_CHUNK_SIZE);
            addr += FLASH_RD_CHUNK_SIZE, chunk_num++) {
            float remain = (float)(100.0 * ((float)(addr - DFLT_APP_START_ADDR))) / ((float)MAX_GENERIC_SIZE);
            printf("\r[i] %5.2f %%", remain);
            for(try_num = 1; try_num <= FLASH_RD_TRY_CNT; try_num++) {
                res =
                    cli_wrp_flash_read(addr, (pFirmwareFromFlash + (addr - DFLT_APP_START_ADDR)), FLASH_RD_CHUNK_SIZE);
                if(res) {
#ifdef HAS_FW_UPDATE_DEBUG
                    printf("\n[*] read ok:");
#endif
                    try_num = FLASH_RD_TRY_CNT * 2;
                } else {
                    printf("\n[w] read flash chunk error. chunk num: %u", chunk_num);
                    if(FLASH_RD_TRY_CNT == try_num) {
                        try_num = FLASH_RD_TRY_CNT * 2;
                        addr = DFLT_APP_END_ADDR + 1;
                        res = false;
                        printf("\n[e] read flash chunk error after %u try. chunk num: %u", FLASH_RD_TRY_CNT, chunk_num);
                    }
                }
            }
        }
        if(out_firmware) {
            *out_firmware = pFirmwareFromFlash;
        }
        if(fw_len) {
            *fw_len = MAX_GENERIC_SIZE;
        }
    } else {
        printf("\n[e] malloc error");
    }
    return res;
}

bool loader_write_firmware(uint8_t* newFirmware, uint32_t new_fw_len) {
    bool res = false;
#ifdef HAS_FW_UPDATE_DEBUG
    printf("\n%s():", __FUNCTION__);
#endif
    if(newFirmware && (0 < new_fw_len)) {
        uint32_t offset = 0;
        for(offset = 0; offset <= (new_fw_len - FLASH_WR_CHUNK_SIZE); offset += FLASH_WR_CHUNK_SIZE) {
            res = cli_wrp_flash_write_verify(APP_START_ADDRESS + offset, &newFirmware[offset], FLASH_WR_CHUNK_SIZE);
            if(res) {
                printf("\n[d] Flash chunk write OK addr 0x%08x len %u byte", APP_START_ADDRESS + offset,
                       FLASH_WR_CHUNK_SIZE);
            } else {
                printf("\n[e] Flash chunk write error 0x%08x", APP_START_ADDRESS + offset);
                break;
            }
        }
    }
    return res;
}

bool loader_update_firmware(uint8_t* newFirmware, uint32_t new_fw_len) {
    bool res = false;
#ifdef HAS_FW_UPDATE_DEBUG
    printf("\n%s():", __FUNCTION__);
#endif
    if(newFirmware && (0 < new_fw_len)) {
        res = cli_wrp_errase_app();
        if(res) {
            printf("\n[i] Application errase Ok!\n");
            res = loader_write_firmware(newFirmware, new_fw_len);
            if(res) {
                printf("\n[i] Application write Ok\n");
            } else {
                printf("\n[e] Application write error\n");
                res = false;
            }
        } else {
            printf("\n[e] Application errase error\n");
            res = false;
        }
    }
    return res;
}

bool is_current_firmware_equal(uint8_t* newFirmware, uint32_t new_fw_len) {
    bool res = false;
#ifdef HAS_FW_UPDATE_DEBUG
    printf("\n%s():", __FUNCTION__);
#endif
    uint32_t addr = DFLT_APP_START_ADDR;
    uint8_t FlashChunk[FLASH_RD_CHUNK_SIZE] = {0};
    printf("\n");
    uint8_t try_num = 0;
    uint32_t chunk_num = 0;
    for(addr = DFLT_APP_START_ADDR, chunk_num = 0; addr <= (DFLT_APP_END_ADDR - FLASH_RD_CHUNK_SIZE);
        addr += FLASH_RD_CHUNK_SIZE, chunk_num++) {
        float remain = (float)(100.0 * ((float)(addr - DFLT_APP_START_ADDR))) / ((float)MAX_GENERIC_SIZE);
        // printf("\r[i] %5.2f %%", remain);
        for(try_num = 1; try_num <= FLASH_RD_TRY_CNT; try_num++) {
            res = cli_wrp_flash_read(addr, FlashChunk, FLASH_RD_CHUNK_SIZE);
            if(res) {
#ifdef HAS_FW_UPDATE_DEBUG
                printf("\n[*] read ok:");
#endif
                int ret = memcmp(FlashChunk, &newFirmware[addr - DFLT_APP_START_ADDR], FLASH_RD_CHUNK_SIZE);
                if(ret) {
#ifdef HAS_FW_UPDATE_DEBUG
                    printf("\n0x");
                    print_mem(FlashChunk, FLASH_RD_CHUNK_SIZE, true, false, true, false);
                    printf("\n0x");
                    print_mem(&newFirmware[addr - DFLT_APP_START_ADDR], FLASH_RD_CHUNK_SIZE, true, false, true, false);
#endif
                    printf("\n[i] Firmware different chunk num: %u", chunk_num);
                    addr = DFLT_APP_END_ADDR + 1;
                    res = false;
                }
                break;
            } else {
                printf("\n[w] read flash chunk error. chunk num: %u", chunk_num);
                if(FLASH_RD_TRY_CNT == try_num) {
                    try_num = FLASH_RD_TRY_CNT * 2;
                    addr = DFLT_APP_END_ADDR + 1;
                    res = false;
                    printf("\n[e] read flash chunk error after %u try. chunk num: %u", FLASH_RD_TRY_CNT, chunk_num);
                }
            }
        }
    }

    return res;
}
