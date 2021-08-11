#include "core_commands.h"

#include <inttypes.h>
#include <stdio.h>
#include <sys_ctrl.h>

#include "base_cmd.h"
#include "convert.h"
#include "core_driver.h"
#include "ctype.h"
#include "data_utils.h"
#include "debug_info.h"
#include "diag_page_nums.h"
#include "diag_report.h"
#include "io_utils.h"
#include "log.h"
#include "ostream.h"
#include "str_utils.h"
#include "sys_tick.h"
#include "table_utils.h"

// extern void intDefaultHandler(void);
extern int main(void);
bool core_diag_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(0 == argc) {
        uint32_t clock;
        /* Obtain the number of the currently executing interrupt. */
        // io_printf("R0: 0x%08x" CRLF, R0);
        io_printf("main: 0x%08p" CRLF, main);
        io_printf("SysTickIntHandler: 0x%08p" CRLF, SysTickIntHandler);
        clock = SysCtrlClockGet();
        io_printf("clock: %u Hz" CRLF, clock);
        // io_printf("IntDefaultHandler: 0x%08p" CRLF, IntDefaultHandler);

        res = true;
    } else {
        LOG_ERROR(SYS, "Usage: cd");
    }
    return res;
}

bool vector_table_command(int32_t argc, char* argv[]) {
    bool res = false;
    uint32_t vectors_table_base = 0x20004e00;
    if(0 == argc) {
        print_vector_table(vectors_table_base);
        res = true;
    } else if(1 == argc) {
        res = true;
        res = try_str2uint32(argv[0], &vectors_table_base);
        if(false == res) {
            LOG_ERROR(SYS, "Unable to extract vectors_table_base %s", argv[0]);
        }

        if(true == res) {
            print_vector_table(vectors_table_base);
        }
    } else {
        LOG_ERROR(SYS, "Usage: cd ");
    }
    return res;
}

bool cmd_soft_reboot(int32_t argc, char* argv[]) {
    (void)(argv);
    bool res = false;
    if(0 == argc) {
        res = true;
        LOG_INFO(SYS, "Reboot device");
        res = reboot();
    } else {
        LOG_ERROR(SYS, "Usage: reboot");
    }
    return res;
}
