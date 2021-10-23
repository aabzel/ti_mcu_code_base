#include "boot_commands.h"

#include <hw_ccfg.h>
#include <hw_memmap.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys_ctrl.h>

#include "base_cmd.h"
#include "bit_utils.h"
#include "boot_cfg.h"
#include "boot_driver.h"
#include "convert.h"
#include "core_driver.h"
#include "ctype.h"
#include "data_utils.h"
#include "debug_info.h"
#include "diag_report.h"
#include "flash_drv.h"
#include "io_utils.h"
#include "log.h"
#include "ostream.h"
#include "read_mem.h"
#include "sys_config.h"
#include "table_utils.h"

static bool boot_scan_app(void) {
    bool res = false;
    bool res1 = false;
    bool res2 = false;
    uint32_t top_stack_val = 0;
    uint32_t reset_handler_addr = 0;
    uint32_t address = 0;
    uint32_t num = 1;
    static const table_col_t cols[] = {{7, "No"}, {12, "address"}, {12, "stack"}, {12, "resetH"}};
    table_header(&dbg_o.s, cols, ARRAY_SIZE(cols));

    for(address = NOR_FLASH_BASE; address < (NOR_FLASH_END - 8); address += 4) {
        top_stack_val = read_addr_32bit(address);
        reset_handler_addr = read_addr_32bit(address + 4);
        res1 = is_ram_addr(top_stack_val);
        res2 = is_flash_addr(reset_handler_addr);
        if(res1 && res2) {
            io_printf(TSEP);
            io_printf(" %5u " TSEP, num);
            io_printf(" 0x%08x " TSEP, address);
            io_printf(" 0x%08x " TSEP, top_stack_val);
            io_printf(" 0x%08x " TSEP, reset_handler_addr);
            io_printf(CRLF);
            num++;
            res = true;
        }
    }
    table_row_bottom(&dbg_o.s, cols, ARRAY_SIZE(cols));
    return res;
}

static bool parse_bl_config(uint32_t reg_val) {
    uint32_t sub_val = 0;
    sub_val = extract_subval_from_32bit(reg_val, 7, 0);
    io_printf("%sables the boot loader backdoor." CRLF, (0xC5 == sub_val) ? "en" : "dis");

    sub_val = extract_subval_from_32bit(reg_val, 15, 8);
    io_printf("level checked at DIO%u" CRLF, sub_val);

    io_printf("Active %s" CRLF, ((1 << 16) == (reg_val & (1 << 16))) ? "high" : "low");

    sub_val = extract_subval_from_32bit(reg_val, 31, 24);
    io_printf("Boot loader is %sabled" CRLF, (0xC5 == sub_val) ? "en" : "dis");
    return true;
}

bool boot_diag_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(0 == argc) {
        //+CCFG_O_BL_CONFIG
        uint32_t reg_val;
        reg_val = read_addr_32bit(CCFG_BASE + CCFG_O_BL_CONFIG);
        res = parse_bl_config(reg_val);
        boot_scan_app();
    } else {
        LOG_ERROR(BOOT, "Usage: bd");
    }
    return res;
}

bool boot_jump_addr_command(int32_t argc, char* argv[]) {
    bool res = false;
    uint32_t app_start_address = 0;
    if(1 == argc) {
        res = try_str2uint32(argv[0], &app_start_address);
        if(false == res) {
            LOG_ERROR(BOOT, "Unable to spot addr", argv[0]);
        }
    } else {
        LOG_ERROR(BOOT, "Usage: jm app_start_address");
    }
    if(res) {
        res = boot_jump_to_code(app_start_address);
    }
    return res;
}

bool bool_erase_app_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(0 == argc) {
        res = flash_erase_pages(APP_PAGE_START, APP_PAGE_START + APP_PAGE_CNT);
        if(res) {
            LOG_ERROR(BOOT, "OK");
        } else {
            LOG_ERROR(BOOT, "Unable app erase");
        }

    } else {
        LOG_ERROR(BOOT, "Usage: ea");
    }
    return res;
}

bool bool_launch_app_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(0 == argc) {
        res = boot_launch_app();
        if(res) {
            LOG_ERROR(BOOT, "OK");
        } else {
            LOG_ERROR(BOOT, "Unable launch app");
        }

    } else {
        LOG_ERROR(BOOT, "Usage: la");
    }
    return res;
}

bool boot_jump_boot_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(0 == argc) {
        res = boot_jump_to_boot();
        if(res) {
            LOG_ERROR(BOOT, "OK");
        } else {
            LOG_ERROR(BOOT, "Unable jump boot");
        }

    } else {
        LOG_ERROR(BOOT, "Usage: jb");
    }
    return res;
}
