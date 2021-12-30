#include "flash_fs_commands.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bit_utils.h"
#include "convert.h"
#include "crc8_autosar.h"
#include "data_utils.h"
#include "debug_info.h"
//#include "diag_report.h"
#include "diag_sys.h"
#include "flash_fs.h"
#include "flash_fs_ll.h"
#include "io_utils.h"
#include "log.h"
#include "memory_layout.h"
#include "none_blocking_pause.h"
#include "str_utils.h"
#include "table_utils.h"
#include "writer_config.h"

static bool print_page_stat(FlashPageFs_t* page, uint8_t num) {
    bool res = false;
    if(page) {
        res = true;
        io_printf("page%u %u files" CRLF, num, page->files_cnt);
        io_printf("page%u %u %% usage" CRLF, num, page->usage_pre_cent);
        io_printf("page%u %u %% free" CRLF, num, 100 - page->usage_pre_cent);
    }
    return res;
}

bool cmd_flash_fs_diag(int32_t argc, char* argv[]) {
    bool res = false;
    if(0 == argc) {
        res = true;
        uint32_t mm_page_start = 0;
        uint32_t mm_page_len = 0;
        uint32_t remaining_space = 0;
        io_printf("page1: 0x%08x" CRLF, MEMORY_MANAGER1_OFFSET);
        io_printf("page2: 0x%08x" CRLF, MEMORY_MANAGER2_OFFSET);
        res = mm_get_active_page(&mm_page_start, &mm_page_len);
        if(res) {
            io_printf("page num: %u" CRLF, addr2page_num(mm_page_start));
            io_printf("mm_page_start 0x%08x" CRLF, mm_page_start);
            io_printf("mm_page_len %u Byte %u kByte" CRLF, mm_page_len, mm_page_len / 1024);
        } else {
            LOG_ERROR(FLASH_FS, "Get active page error");
        }
        remaining_space = mm_get_remaining_space();
        io_printf("remaining_space %u Byte %u kByte" CRLF, remaining_space, remaining_space / 1024);

        print_page_stat(&FlashFs.page1, 1);
        print_page_stat(&FlashFs.page2, 2);

    } else {
        LOG_ERROR(FLASH_FS, "Usage: ffd");
    }
    return res;
}

bool cmd_flash_fs_get(int32_t argc, char* argv[]) {
    bool res = false;
    unsigned char array[100] = "";
    uint16_t file_id = 0;
    uint16_t file_len = 0;
    if(1 == argc) {
        res = true;

        res = try_str2uint16(argv[0], &file_id);
        if(false == res) {
            LOG_ERROR(FLASH_FS, "Unable to extract sector_num %s", argv[0]);
        }

        if(true == res) {
            res = mm_get(file_id, array, sizeof(array), &file_len);
            if(false == res) {
                LOG_ERROR(FLASH_FS, "mm_get error");
            } else {
                print_mem((uint8_t*)array, (uint16_t)file_len, true, true, false, true);
                io_printf(CRLF);
            }
        }
    } else if(0 == argc) {
        static const table_col_t cols[] = {{7, "id"}, {12, "addr"}, {5, "data"}};
        table_header(&(curWriterPtr->s), cols, ARRAY_SIZE(cols));

        for(file_id = 0; file_id < 0xFFFF; file_id++) {
            file_len = 0;
            uint32_t* file_Address = NULL;
            res = mm_get((uint16_t)file_id, array, sizeof(array), &file_len);
            if(true == res) {
                res = mm_get_address(file_id, (uint8_t**)&file_Address, &file_len);
            }
            if((true == res) && (0 < file_len)) {
                io_printf("| %5u | 0x%08p | ", file_id, file_Address);
                print_bin(array, file_len, 0);
                print_ascii_line((char*)array, file_len, 1);
                io_printf(CRLF);
                res = true;
            }
        }
        table_row_bottom(&(curWriterPtr->s), cols, ARRAY_SIZE(cols));
    } else {
        LOG_ERROR(FLASH_FS, "Usage: ffg");
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
                LOG_ERROR(FLASH_FS, "Unable to extract sector_num %s", argv[0]);
            }
        }
        if(true == res) {
            uint16_t file_len = 0;
            uint32_t* file_Address = NULL;
            res = mm_get_address(file_id, (uint8_t**)&file_Address, &file_len);
            if(false == res) {
                LOG_ERROR(FLASH_FS, "mm_get error");
            } else {
                io_printf("Addr: %p Len: %u" CRLF, file_Address, file_len);
            }
        }
    } else {
        LOG_ERROR(FLASH_FS, "Usage: ffg");
    }
    return res;
}

bool cmd_flash_fs_set(int32_t argc, char* argv[]) {
    bool res = false;
    if(2 == argc) {
        uint32_t write_len = 0;
        res = true;
        bool is_text = false;
        Type32Union_t union_data = {0};
        uint8_t text[80] = "";
        uint16_t file_id = 0;
        if(true == res) {
            res = try_str2uint16(argv[0], &file_id);
            if(false == res) {
                LOG_ERROR(FLASH_FS, "Unable to extract file_id %s", argv[0]);
            }
        }

        if(0 == write_len) {
            res = try_str2uint8(argv[1], &union_data.u8[0]);
            if(true == res) {
                LOG_INFO(FLASH_FS, "Spot uint8");
                write_len = 1;
            }
        }

        if(0 == write_len) {
            res = try_str2uint16(argv[1], &union_data.u16[0]);
            if(true == res) {
                LOG_INFO(FLASH_FS, "Spot uint16");
                write_len = 2;
            }
        }

        if(0 == write_len) {
            res = try_str2uint32(argv[1], &union_data.u32);
            if(true == res) {
                LOG_INFO(FLASH_FS, "Spot uint32");
                write_len = 4;
            }
        }
        if(0 == write_len) {
            strncpy((char*)text, argv[1], sizeof(text));
            write_len = strlen((char*)text) + 1;
            LOG_INFO(FLASH_FS, "Spot text");
            is_text = true;
        }

        if(0 < write_len) {
            if(is_text) {
                res = mm_set(file_id, (uint8_t*)&text, write_len);
            } else {
                res = mm_set(file_id, (uint8_t*)&union_data, write_len);
            }
            if(false == res) {
                LOG_ERROR(FLASH_FS, "mm_set error");
            } else {
                LOG_INFO(FLASH_FS, "mm_set OK");
            }
        } else {
            LOG_ERROR(FLASH_FS, "mm_set len error");
        }
    } else {
        LOG_ERROR(FLASH_FS, "Usage: ffs id val");
    }
    return res;
}

bool cmd_flash_fs_format(int32_t argc, char* argv[]) {
    bool res = false;
    if(0 == argc) {
        res = true;
        res = mm_flash_format();
        if(false == res) {
            LOG_ERROR(FLASH_FS, "mmi flash format error");
        } else {
            LOG_INFO(FLASH_FS, "mmi flash format OK");
        }
    } else {
        LOG_ERROR(FLASH_FS, "Usage: fff");
    }
    return res;
}

bool cmd_flash_fs_toggle_page(int32_t argc, char* argv[]) {
    bool res = false;
    if(0 == argc) {
        res = true;
        res = mm_turn_page();
        if(false == res) {
            LOG_ERROR(FLASH_FS, "Unable to turn page");
        } else {
            LOG_INFO(FLASH_FS, "Toggle page OK");
        }
    } else {
        LOG_ERROR(FLASH_FS, "Usage: fft");
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
                LOG_ERROR(FLASH_FS, "Unable to extract sector_num %s", argv[0]);
            }
        }
        if(true == res) {
            res = mm_invalidate(file_id);
            if(false == res) {
                LOG_ERROR(FLASH_FS, "invalidate error");
            } else {
                io_printf("id %u invalidate OK" CRLF, file_id);
            }
        }
    } else {
        LOG_ERROR(FLASH_FS, "Usage: ffi id");
    }
    return res;
}

static bool flash_fs_scan(uint32_t start_page_addr, uint32_t page_len, char* key_word1, char* key_word2) {
    bool res = false;
    bool out_res = false;
    mmItem_t* item = NULL;
    uint32_t rem_size = 0;
    uint16_t num = 0;
    uint32_t cur_offset = start_page_addr;
    uint32_t file_cnt = 0;
    char temp_str[120];
    LOG_INFO(FLASH_FS, "Page 0x%08x...0x%08x", start_page_addr, start_page_addr + page_len);
    uint8_t* payload = NULL;
    static const table_col_t cols[] = {{5, "num"}, {7, "id"}, {7, "len"}, {6, "crc"}, {12, "addr"}, {5, "data"}};
    table_header(&(curWriterPtr->s), cols, ARRAY_SIZE(cols));
    uint8_t data[50] = "";
    for(cur_offset = start_page_addr; cur_offset <= (start_page_addr + page_len - MIN_FILE_LEN); cur_offset++) {
        /* reference item */
        res = true;
        item = (mmItem_t*)cur_offset;

        /*is valid id*/
        if(item->id != (MASK_16BIT ^ item->nid)) {
            res = false;
        }

        if(res) {
            rem_size = start_page_addr + page_len - cur_offset;
            /*is data with that length can be fitted in flash page?*/
            if(rem_size < item->length) {
                res = false;
            }
            if(0 == item->length) {
                res = false;
            }
        }

        if(res) {
            payload = (uint8_t*)(cur_offset + ITEM_HEADER_SZ);
            uint8_t calc_crc8 = crc8_autosar_calc((void*)(payload), item->length);
            if(calc_crc8 != item->crc8) {
                res = false;
            }
        }

        if(res) {
            /*spot data file valid item */
            file_cnt++;
            strcpy(temp_str, TSEP);
            snprintf(temp_str, sizeof(temp_str), "%s %5u " TSEP, temp_str, item->id);
            snprintf(temp_str, sizeof(temp_str), "%s %5u " TSEP, temp_str, item->length);
            snprintf(temp_str, sizeof(temp_str), "%s 0x%02x " TSEP, temp_str, item->crc8);
            snprintf(temp_str, sizeof(temp_str), "%s 0x%08p " TSEP, temp_str, item);
            res = hex2ascii(payload, item->length, data, sizeof(data));
            if(res) {
                snprintf(temp_str, sizeof(temp_str), "%s 0x%s " TSEP, temp_str, data);
            }
            snprintf(temp_str, sizeof(temp_str), "%s" CRLF, temp_str);
            if(is_contain(temp_str, key_word1, key_word2)) {
                res = true;
            }
        }
        if(res) {
            io_printf(TSEP " %3u ", num);
            io_printf("%s", temp_str);
            num++;
        }
    }
    table_row_bottom(&(curWriterPtr->s), cols, ARRAY_SIZE(cols));
    if(0 < file_cnt) {
        out_res = true;
    }
    return out_res;
}

bool cmd_flash_fs_scan(int32_t argc, char* argv[]) {
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
        LOG_ERROR(FLASH_FS, "Usage: ffc");
    }

    if(res) {
        res = flash_fs_scan(MEMORY_MANAGER1_OFFSET, MEMORY_MANAGER1_LENGTH, keyWord1, keyWord2);
        if(false == res) {
            LOG_ERROR(FLASH_FS, "scan error");
        }
        res = flash_fs_scan(MEMORY_MANAGER2_OFFSET, MEMORY_MANAGER2_LENGTH, keyWord1, keyWord2);
        if(false == res) {
            LOG_ERROR(FLASH_FS, "scan error");
        }
    }

    return res;
}

bool cmd_flash_fs_init(int32_t argc, char* argv[]) {
    bool res = false;
    if(0 == argc) {
        res = flash_fs_init();
        if(false == res) {
            LOG_ERROR(FLASH_FS, "InitErr");
        } else {
            LOG_INFO(FLASH_FS, "InitOk");
        }
    }
    return res;
}
