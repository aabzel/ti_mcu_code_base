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
    if(0 == argc) {
        res = true;
        unsigned int mmPageStart;
        unsigned int mmPageLen;
        int remaining_space = 0;
        res = mm_get_active_page(&mmPageStart, &mmPageLen);
        if(res) {
            io_printf("mmPageStart %08x" CRLF, mmPageStart);
            io_printf("mmPageLen %u Byte %u kByte" CRLF, mmPageLen, mmPageLen / 1024);
        }
        remaining_space = mm_get_remaining_space();
        io_printf("remaining_space %u Byte %u kByte" CRLF, remaining_space, remaining_space / 1024);
    } else {
        LOG_ERROR(SYS, "Usage: ffd ");
    }
    return res;
}

bool cmd_flash_fs_get(int32_t argc, char* argv[]) {
    bool res = false;
    unsigned char array[100];
    uint16_t file_id = 0;
    uint16_t file_len = 0;
    if(1 == argc) {
        res = true;
        if(true == res) {
            res = try_str2uint16(argv[0], &file_id);
            if(false == res) {
                LOG_ERROR(SYS, "Unable to extract sector_num %s", argv[0]);
            }
        }
        if(true == res) {
            res = mm_get(file_id, array, sizeof(array), &file_len);
            if(false == res) {
                LOG_ERROR(SYS, "mm_get error");
            } else {
                print_mem((uint8_t*)array, (uint16_t)file_len, false);
                io_printf(CRLF);
            }
        }
    } else if(0 == argc) {
        static const table_col_t cols[] = {{7, "id"}, {12, "addr"}, {5, "data"}};
        table_header(&dbg_o.s, cols, ARRAY_SIZE(cols));

        for(file_id = 0; file_id < 0xFFFF; file_id++) {
            file_len = 0;
            uint32_t* file_Address = NULL;
            res = mm_get((uint16_t)file_id, array, sizeof(array), &file_len);
            if(true == res) {
                res = mm_get_address(file_id, (uint8_t**)&file_Address, &file_len);
            }
            if((true == res) && ( 0<file_len)) {
                io_printf("| %5u | 0x%08p | ", file_id, file_Address);
                print_bin(array, file_len, 0);
                print_ascii_line((char*)array, file_len, 1);
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
    if(1 == argc) {
        res = true;
        uint16_t file_id = 0;
        if(true == res) {
            res = try_str2uint16(argv[0], &file_id);
            if(false == res) {
                LOG_ERROR(SYS, "Unable to extract sector_num %s", argv[0]);
            }
        }
        if(true == res) {
            uint16_t file_len = 0;
            uint32_t* file_Address = NULL;
            res = mm_get_address(file_id, (uint8_t**)&file_Address, &file_len);
            if(false == res) {
                LOG_ERROR(SYS, "mm_get error");
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
    if(2 == argc) {
        uint32_t write_len = 0;
        res = true;
        bool is_text = false;
        Type32Union_t union_data;
        uint8_t text[80] = "";
        uint16_t file_id = 0;
        if(true == res) {
            res = try_str2uint16(argv[0], &file_id);
            if(false == res) {
                LOG_ERROR(SYS, "Unable to extract file_id %s", argv[0]);
            }
        }

        if(0 == write_len) {
            res = try_str2uint8(argv[1], &union_data.u8[0]);
            if(true == res) {
                write_len = 1;
            }
        }

        if(0 == write_len) {
            res = try_str2uint16(argv[1], &union_data.u16[0]);
            if(true == res) {
                write_len = 2;
            }
        }

        if(0 == write_len) {
            res = try_str2uint32(argv[1], &union_data.u32);
            if(true == res) {
                write_len = 4;
            }
        }
        if(0 == write_len) {
            strncpy((char*)text, argv[1], sizeof(text));
            write_len = strlen((char*)text) + 1;
            is_text = true;
        }

        if(0 < write_len) {
            if(is_text) {
                res = mm_set(file_id, (uint8_t*)&text, write_len);
            } else {
                res = mm_set(file_id, (uint8_t*)&union_data, write_len);
            }
            if(false == res) {
                LOG_ERROR(SYS, "mm_set error");
            } else {
                LOG_INFO(SYS, "mm_set OK");
            }
        } else {
            LOG_ERROR(SYS, "mm_set len error");
        }
    } else {
        LOG_ERROR(SYS, "Usage: ffs id val");
    }
    return res;
}

bool cmd_flash_fs_format(int32_t argc, char* argv[]) {
    bool res = false;
    if(0 == argc) {
        res = true;
        res = mm_flash_format();
        if(false == res) {
            LOG_ERROR(SYS, "mmi flash format error");
        } else {
            LOG_INFO(SYS, "mmi flash format OK");
        }
    } else {
        LOG_ERROR(SYS, "Usage: fff");
    }
    return res;
}

bool cmd_flash_fs_toggle_page(int32_t argc, char* argv[]) {
    bool res = false;
    if(0 == argc) {
        res = true;
        res = mm_turn_page();
        if(false == res) {
            LOG_ERROR(SYS, "Unable to turn page %d" CRLF);
        } else {
            LOG_INFO(SYS, "Toggle page OK");
        }
    } else {
        LOG_ERROR(SYS, "Usage: fft");
    }
    return res;
}

bool cmd_flash_fs_inval(int32_t argc, char* argv[]) {
    bool res = false;

    if(1 == argc) {
        res = true;
        uint16_t file_id = 0;
        if(true == res) {
            res = try_str2uint16(argv[0], &file_id);
            if(false == res) {
                LOG_ERROR(SYS, "Unable to extract sector_num %s", argv[0]);
            }
        }
        if(true == res) {
            res = mm_invalidate(file_id);
            if(false == res) {
                LOG_ERROR(SYS, "invalidate error");
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
