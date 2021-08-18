#include "boot_commands.h"

#include <hw_ccfg.h>
#include <hw_memmap.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys_ctrl.h>

#include "base_cmd.h"
#include "bit_utils.h"
#include "boot_driver.h"
#include "convert.h"
#include "ctype.h"
#include "data_utils.h"
#include "debug_info.h"
#include "diag_report.h"
#include "io_utils.h"
#include "log.h"
#include "ostream.h"
#include "read_mem.h"

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
    } else {
        LOG_ERROR(SYS, "Usage: btd");
    }
    return res;
}
