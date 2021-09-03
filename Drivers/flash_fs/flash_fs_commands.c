#include "flash_fs_commands.h"

#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

#include "bit_utils.h"
#include "convert.h"
#include "data_utils.h"
#include "debug_info.h"
#include "diag_report.h"
#include "diag_sys.h"
#include "flash_fs.h"
#include "flash_fs_ll.h"
#include "io_utils.h"
#include "log.h"
#include "none_blocking_pause.h"
#include "table_utils.h"

bool cmd_flash_fs_diag(int32_t argc, char* argv[]) {
    bool res = false;
    if (0 == argc) {
        res = true;
        unsigned int mmPageStart;
        unsigned int mmPageLen;
        int remaining_space = 0;
        int ret = mmiGetActivePage(&mmPageStart, &mmPageLen);
        io_printf("ret %u" CRLF, ret);
        io_printf("mmPageStart %08x" CRLF, mmPageStart);
        io_printf("mmPageLen %u Byte %u kByte" CRLF, mmPageLen, mmPageLen / 1024);
        remaining_space = mm_getRemainingSpace();
        io_printf("remaining_space %u Byte %u kByte" CRLF, remaining_space, remaining_space / 1024);
    } else {
        LOG_ERROR(SYS, "Usage: ffd ");
    }
    return res;
}

bool cmd_flash_fs_get(int32_t argc, char* argv[]) {
    bool res = false;
    unsigned char array[100];
    if (1 == argc) {
        res = true;
        uint16_t file_id = 0;
        if (true == res) {
            res = try_str2uint16(argv[0], &file_id);
            if (false == res) {
                LOG_ERROR(SYS, "Unable to extract sector_num %s", argv[0]);
            }
        }
        if (true == res) {
            uint16_t file_len = 0;
            int ret = mm_get(file_id, array, sizeof(array), &file_len);
            if (MM_RET_CODE_OK != ret) {
                LOG_ERROR(SYS, "mm_get error");
                io_printf("ret %u" CRLF, ret);
                res = false;
            } else {
                print_mem((uint8_t*)array, (uint16_t)file_len, false);
                io_printf(CRLF);
            }
        }
    } else if (0 == argc) {
        uint32_t file_id = 0;
        static const table_col_t cols[] = {{7, "id"}, {12, "addr"}, {5, "data"}};
        uint16_t file_len = 0;
        table_header(&dbg_o.s, cols, ARRAY_SIZE(cols));

        for (file_id = 0; file_id <= 0xFFFF; file_id++) {
            file_len = 0;
            uint32_t* file_Address = NULL;
            int ret = mm_get((uint16_t)file_id, array, sizeof(array), &file_len);
            if (MM_RET_CODE_OK == ret) {

                ret = mm_getAddress(file_id, (uint8_t**)&file_Address, &file_len);
            }
            if (MM_RET_CODE_OK == ret) {
                io_printf("| %5u | 0x%08p | ", file_id, file_Address);
                print_mem((uint8_t*)array, (uint16_t)file_len, false);
                io_printf(CRLF);
                res = true;
            }
        }
        table_row_bottom(&dbg_o.s, cols, ARRAY_SIZE(cols));
    } else {
        LOG_ERROR(SYS, "Usage: ffg");
    }
    return res;
}

bool cmd_flash_fs_get_addr(int32_t argc, char* argv[]) {
    bool res = false;
    if (1 == argc) {
        res = true;
        uint16_t file_id = 0;
        if (true == res) {
            res = try_str2uint16(argv[0], &file_id);
            if (false == res) {
                LOG_ERROR(SYS, "Unable to extract sector_num %s", argv[0]);
            }
        }
        if (true == res) {
            uint16_t file_len = 0;
            uint32_t* file_Address = NULL;
            int ret = mm_getAddress(file_id, (uint8_t**)&file_Address, &file_len);
            if (MM_RET_CODE_OK != ret) {
                LOG_ERROR(SYS, "mm_get error");
                io_printf("ret %u" CRLF, ret);
                res = false;
            } else {
                io_printf("Addr: %p Len: %u" CRLF, file_Address, file_len);
            }
        }
    } else {
        LOG_ERROR(SYS, "Usage: ffg");
    }
    return res;
}

bool cmd_flash_fs_set(int32_t argc, char* argv[]) {
    bool res = false;
    if (2 == argc) {
        uint32_t write_len = 0;
        res = true;
        Type32Union_t union_data;
        uint16_t file_id = 0;
        if (true == res) {
            res = try_str2uint16(argv[0], &file_id);
            if (false == res) {
                LOG_ERROR(SYS, "Unable to extract file_id %s", argv[0]);
            }
        }

        if (0 == write_len) {
            res = try_str2uint8(argv[1], &union_data.u8[0]);
            if (true == res) {
                write_len = 1;
            }
        }

        if (0 == write_len) {
            res = try_str2uint16(argv[1], &union_data.u16[0]);
            if (true == res) {
                write_len = 2;
            }
        }

        if (0 == write_len) {
            res = try_str2uint32(argv[1], &union_data.u32);
            if (true == res) {
                write_len = 4;
            }
        }

        if (0 < write_len) {
            int ret = mm_set(file_id, (unsigned char*)&union_data, write_len);
            if (MM_RET_CODE_OK != ret) {
                LOG_ERROR(SYS, "mm_set error");
                io_printf("ret %u" CRLF, ret);
                res = false;
            } else {
                LOG_INFO(SYS, "mm_set OK");
            }
        }
    } else {
        LOG_ERROR(SYS, "Usage: ffs id val");
    }
    return res;
}

bool cmd_flash_fs_format(int32_t argc, char* argv[]) {
    bool res = false;
    if (0 == argc) {
        res = true;
        int ret;
        ret = mmiFlashFormat();
        if (ret != MM_RET_CODE_OK) {
            res = false;
            LOG_ERROR(SYS,"mmi flash format error %d" , ret);
        } else {
            LOG_INFO(SYS,"mmi flash format OK" );
            res = true;
        }
    } else {
        LOG_ERROR(SYS, "Usage: fff ");
    }
    return res;
}

bool cmd_flash_fs_toggle_page(int32_t argc, char* argv[]) {
    bool res = false;
    if (0 == argc) {
        res = true;
        int ret;
        ret = mm_turnThePage();
        if (ret != MM_RET_CODE_OK) {
            res = false;
            LOG_ERROR(SYS, "Toggle page error");
            io_printf("Unable to turn page %d" CRLF, ret);
        } else {
            LOG_INFO(SYS, "Toggle page OF");
            res = true;
        }
    } else {
        LOG_ERROR(SYS, "Usage: fft");
    }
    return res;
}

bool cmd_flash_fs_inval(int32_t argc, char* argv[]) {
    bool res = false;

    if (1 == argc) {
        res = true;
        uint16_t file_id = 0;
        if (true == res) {
            res = try_str2uint16(argv[0], &file_id);
            if (false == res) {
                LOG_ERROR(SYS, "Unable to extract sector_num %s", argv[0]);
            }
        }
        if (true == res) {
            int ret = mm_invalidate(file_id);
            if (MM_RET_CODE_OK != ret) {
                LOG_ERROR(SYS, "invalidate error");
                io_printf("ret %u" CRLF, ret);
                res = false;
            } else {
                io_printf("id %u invalidate OK" CRLF, file_id);
            }
        }
    } else {
        LOG_ERROR(SYS, "Usage: ffi id");
    }
    return res;
}
