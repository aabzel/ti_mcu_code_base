#include "flash_nvs_commands.h"

#include <flash.h>
#include <inttypes.h>
#include <string.h>
#include <stdio.h>
#include <vims.h>

#include "convert.h"
#include "crc16_ccitt.h"
#include "crc32.h"
#include "data_utils.h"
#include "debug_info.h"
#include "diag_page_nums.h"
#include "diag_report.h"
#include "flash_nvs_drv.h"
#include "log.h"
#include "str_utils.h"
#include "table_utils.h"

bool flash_nvs_diag_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(0 == argc) {
        res = true;
    }

    if(0 < argc) {
        LOG_ERROR(NVS, "Usage: fd key1 key2");
    }

    if(res) {
        /* Display the NVS region attributes */
        io_printf("Base Address: 0x%08x" CRLF, regionAttrs.regionBase);
        io_printf("Sector Size : 0x%08x" CRLF, regionAttrs.sectorSize);
        io_printf("Region Size : 0x%08x" CRLF, regionAttrs.regionSize);

    }

    return res;
}

bool flash_nvs_erase_command(int32_t argc, char* argv[]) {
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

bool flash_nvs_read_command(int32_t argc, char* argv[]) {
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


bool flash_nvs_write_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(2 == argc) {
        res = true;
        uint32_t offset = 0;
        uint8_t array[256]={0};
        memset(array,0xFF,sizeof(array));
        uint32_t array_len = 0;
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

