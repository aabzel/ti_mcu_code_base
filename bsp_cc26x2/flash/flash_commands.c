#include "flash_commands.h"

#include <inttypes.h>
#include <stdio.h>

#include "convert.h"
#include "crc32.h"
#include "data_utils.h"
#include "debug_info.h"
#include "diag_page_nums.h"
#include "diag_report.h"
#include "flash_drv.h"
#include "log.h"
#include "str_utils.h"
#include "table_utils.h"

static bool diag_flash_prot(char* key_word1, char* key_word2) {
    bool res = false;
    uint32_t flash_addr = NOR_FLASH_BASE, num = 0;
    char line_str[120];
    static const table_col_t cols[] = {{5, "num"}, {10, "start"}, {10, "end"}, {8, "status"}};
    table_header(&dbg_o.s, cols, ARRAY_SIZE(cols));
    for(flash_addr = 0; flash_addr < NOR_FLASH_SIZE; flash_addr += FLASH_SECTOR_SIZE) {
        strcpy(line_str, TSEP);
        snprintf(line_str, sizeof(line_str), "%s 0x%08x" TSEP, line_str, flash_addr);
        snprintf(line_str, sizeof(line_str), "%s 0x%08x" TSEP, line_str, flash_addr + FLASH_SECTOR_SIZE);
        res = is_addr_protected(flash_addr);
        if(res) {
            snprintf(line_str, sizeof(line_str), "%s protected" TSEP, line_str);
        } else {
            snprintf(line_str, sizeof(line_str), "%s WrEnable" TSEP, line_str);
        }
        snprintf(line_str, sizeof(line_str), "%s" CRLF, line_str);
        if(is_contain(line_str, key_word1, key_word2)) {
            io_printf(TSEP " %3u ", num);
            io_printf("%s", line_str);
            num++;
        }
    }

    return res;
}

bool flash_diag_command(int32_t argc, char* argv[]) {
    bool res = false;
    char keyWord1[20] = "";
    char keyWord2[20] = "";
    if(0 <= argc) {
        strncpy(keyWord1, "", sizeof(keyWord1));
        strncpy(keyWord2, "", sizeof(keyWord2));
        res = true;
    }
    if(1 <= argc) {
        strncpy(keyWord1, argv[0], sizeof(keyWord1));
        res = true;
    }
    if(2 <= argc) {
        strncpy(keyWord2, argv[1], sizeof(keyWord2));
        res = true;
    }

    if(2 < argc) {
        LOG_ERROR(NVS, "Usage: fd key1 key2");
    }

    if(res) {
        uint32_t all_flash_crc = 0;
        // FlashSizeGet
        // FlashPowerModeGet
        // FlashProtectionGet
        all_flash_crc = crc32(((uint8_t*)NOR_FLASH_BASE), NOR_FLASH_SIZE);
        io_printf("FlashCRC32: 0x%08x" CRLF, all_flash_crc);

        io_printf("Flash Base: 0x%08x" CRLF, NOR_FLASH_BASE);
        io_printf("Flash size: %u byte %u kByte" CRLF, NOR_FLASH_SIZE, NOR_FLASH_SIZE / 1024);
        io_printf("Flash End: 0x%08x" CRLF, NOR_FLASH_END - 1);

        /* Display the NVS region attributes */
        io_printf("Base Address: 0x%08x" CRLF, regionAttrs.regionBase);
        io_printf("Sector Size : 0x%08x" CRLF, regionAttrs.sectorSize);
        io_printf("Region Size : 0x%08x" CRLF, regionAttrs.regionSize);
        res = diag_flash_prot(keyWord1, keyWord2);
    }

    return res;
}

bool flash_erase_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(2 == argc) {
        res = true;
        uint32_t offset = 0;
        uint32_t sector_size = 0;
        if(res) {
            res = try_str2uint32(argv[0], &offset);
            if(false == res) {
                LOG_ERROR(NVS, "Unable to parse offset %s", argv[0]);
            }
        }
        if(res) {
            res = try_str2uint32(argv[1], &sector_size);
            if(false == res) {
                LOG_ERROR(NVS, "Unable to parse sector_size %s", argv[1]);
            }
        }
        if(res) {
            int_fast16_t ret;
            ret = NVS_erase(nvsHandle, 0, sector_size);
            if(NVS_STATUS_SUCCESS != ret) {
                LOG_ERROR(NVS, "NVS_erase error");
                io_printf("ret: %u" CRLF, ret);
            } else {
                LOG_INFO(NVS, "NVS_erase OK");
            }
        }
    } else {
        LOG_ERROR(NVS, "Usage: fe offset sector_size");
    }
    return res;
}

bool flash_read_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(2 == argc) {
        res = true;
        uint32_t offset = 0;
        uint32_t buffer_size = 0;
        if(res) {
            res = try_str2uint32(argv[0], &offset);
            if(false == res) {
                LOG_ERROR(NVS, "Unable to parse offset %s", argv[0]);
            }
        }
        if(res) {
            res = try_str2uint32(argv[1], &buffer_size);
            if(false == res) {
                LOG_ERROR(NVS, "Unable to parse buffer_size %s", argv[1]);
            }
        }

        if((true == res) && (buffer_size <= sizeof(nvs_buffer))) {
            int_fast16_t ret;
            ret = NVS_read(nvsHandle, offset, (void*)nvs_buffer, buffer_size);
            if(NVS_STATUS_SUCCESS != ret) {
                LOG_ERROR(NVS, "NVS_read error");
                io_printf("ret: %u" CRLF, ret);
            } else {
                print_mem(nvs_buffer, buffer_size, true);
            }
        }
    } else {
        LOG_ERROR(NVS, "Usage: fr offset buffer_size");
    }
    return res;
}

bool flash_write_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(2 == argc) {
        res = true;
        uint32_t offset = 0;
        uint8_t array[256];
        uint16_t array_len = 0;
        if(res) {
            res = try_str2uint32(argv[0], &offset);
            if(false == res) {
                LOG_ERROR(NVS, "Unable to parse offset %s", argv[0]);
            }
        }

        if(true == res) {
            res = try_str2array(argv[1], array, sizeof(array), &array_len);
            if(false == res) {
                LOG_ERROR(NVS, "Unable to extract array %s", argv[1]);
            }
        }
        if(res) {
            int_fast16_t ret;
            ret = NVS_write(nvsHandle, offset, (void*)array, array_len, NVS_WRITE_ERASE | NVS_WRITE_POST_VERIFY);
            if(NVS_STATUS_SUCCESS != ret) {
                LOG_ERROR(NVS, "NVS_write error");
                io_printf("ret: %u" CRLF, ret);
            } else {
                io_printf("OK!" CRLF);
            }
        }
    } else {
        LOG_ERROR(NVS, "Usage: fw offset hex_string");
        LOG_INFO(NVS, "offset");
        LOG_INFO(NVS, "hex_string 0x[0...F]+");
    }
    return res;
}

bool flash_scan_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(0 == argc) {
        float usage_pec = 0.0f;
        uint32_t spare = 0;
        uint32_t busy = 0;
        res = flash_scan((uint8_t*)NOR_FLASH_BASE, NOR_FLASH_SIZE, &usage_pec, &spare, &busy);
        if(res) {
            io_printf("usage: %f %%" CRLF, usage_pec);
            io_printf("spare: %u Bytes %u kBytes" CRLF, spare, spare / 1024);
            io_printf("busy : %u Bytes %u kBytes" CRLF, busy, busy / 1024);
        }

    } else {
        LOG_ERROR(NVS, "Usage: fs");
    }
    return res;
}
