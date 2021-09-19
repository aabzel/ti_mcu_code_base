#include "flash_commands.h"

#include <inttypes.h>
#include <stdio.h>

#include "convert.h"
#include "crc16_ccitt.h"
#include "crc32.h"
#include "data_utils.h"
#include "debug_info.h"
#include "flash_drv.h"
#include "log.h"
#include "str_utils.h"
#include "table_utils.h"

static bool diag_flash_prot(char* key_word1, char* key_word2) {
    bool res = false;
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
        LOG_ERROR(LG_FLASH, "Usage: fd key1 key2");
    }

    if(res) {
        uint32_t all_flash_crc = 0;
        all_flash_crc = crc32(((uint8_t*)NOR_FLASH_BASE), NOR_FLASH_SIZE);
        io_printf("FlashCRC32: 0x%08x" CRLF, all_flash_crc);



        io_printf("Flash Base: 0x%08x" CRLF, NOR_FLASH_BASE);
        io_printf("Flash size: %u byte %u kByte" CRLF, NOR_FLASH_SIZE, NOR_FLASH_SIZE / K_BYTES);
        io_printf("Flash End: 0x%08x" CRLF, NOR_FLASH_END - 1);

        res = diag_flash_prot(keyWord1, keyWord2);
    }

    return res;
}

bool flash_erase_command(int32_t argc, char* argv[]) {
    uint32_t sector_address = 0;
    bool res = false;
    if(1 == argc) {
        res = true;
        res = try_str2uint32(argv[0], &sector_address);
        if(false == res) {
            LOG_ERROR(LG_FLASH, "Unable to parse sector_address %s", argv[0]);
        }
    } else {
        LOG_ERROR(LG_FLASH, "Usage: fe sector_address");
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
                LOG_ERROR(LG_FLASH, "Unable to parse offset %s", argv[0]);
            }
        }
        if(res) {
            res = try_str2uint32(argv[1], &buffer_size);
            if(false == res) {
                LOG_ERROR(LG_FLASH, "Unable to parse buffer_size %s", argv[1]);
            }
        }

    } else {
        LOG_ERROR(LG_FLASH, "Usage: fr offset buffer_size");
    }
    return res;
}

bool flash_write_command(int32_t argc, char* argv[]) {
    bool res = false;
    uint16_t crc16_read = 0;
    uint32_t flash_address = 0;
    uint32_t count = 0;
    uint8_t DataBuffer[256];
    memset(DataBuffer, 0xFF, sizeof(DataBuffer));
    if(1 <= argc) {
        res = try_str2uint32(argv[0], &flash_address);
        if(false == res) {
            LOG_ERROR(LG_FLASH, "Unable to parse sector_address %s", argv[0]);
        } else {
            res = is_flash_addr(flash_address);
            if(false == res) {
                LOG_ERROR(LG_FLASH, "not flash addr 0x%08x", flash_address);
            }
        }
    }
    if(2 <= argc) {
        res = try_str2array(argv[1], DataBuffer, sizeof(DataBuffer), &count);
        if(false == res) {
            LOG_ERROR(LG_FLASH, "Unable to extract hex_string %s", argv[1]);
        }
    }

    if(3 <= argc) {
        res = try_str2uint16(argv[2], &crc16_read);
        if(false == res) {
            LOG_ERROR(LG_FLASH, "Unable to parse crc16_read %s", argv[2]);
        } else {
            res = crc16_check(DataBuffer, count, crc16_read);
            if(false == res) {
                LOG_ERROR(LG_FLASH, "crc16 error");
            }
        }
    }

    if(3 < argc) {
        LOG_ERROR(LG_FLASH, "Usage: fw sector_address hex_string crc16_read");
        LOG_INFO(LG_FLASH, "sector_address");
        LOG_INFO(LG_FLASH, "hex_string 0x[0...F]+");
        LOG_INFO(LG_FLASH, "crc16_read");
    }

    if(res) {
        res = flash_wr(flash_address, DataBuffer, count);
        if(res) {
            LOG_ERROR(LG_FLASH, "FlashProgram ok");
        } else {
            LOG_ERROR(LG_FLASH, "FlashProgram error");
        }
    }
    return res;
}

bool flash_scan_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(0 == argc) {
        float usage_pec = 0.0f;
        uint32_t spare = 0;
        uint32_t busy = 0;
        res = mem_scan((uint8_t*)NOR_FLASH_BASE, NOR_FLASH_SIZE, &usage_pec, &spare, &busy);
        if(res) {
            io_printf("usage: %f %%" CRLF, usage_pec);
            io_printf("spare: %u Bytes %u kBytes" CRLF, spare, spare / 1024);
            io_printf("busy : %u Bytes %u kBytes" CRLF, busy, busy / 1024);
        }
    } else {
        LOG_ERROR(LG_FLASH, "Usage: fs");
    }
    return res;
}
