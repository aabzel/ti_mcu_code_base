#include "core_commands.h"

#include <inttypes.h>
#include <stdio.h>

#include "convert.h"
#include "ctype.h"
#include "data_utils.h"
#include "debug_info.h"
#include "diag_page_nums.h"
#include "diag_report.h"
#include "core_driver.h"
#include "io_utils.h"
#include "log.h"
#include "ostream.h"
#include "shell.h"
#include "sys_tick.h"
#include "table_utils.h"
#include "str_utils.h"

//extern void intDefaultHandler(void);
extern int main(void) ;
bool core_diag_command(int32_t argc, char* argv[]) {
    bool res = false;
    if (0 == argc) {
	/* Obtain the number of the currently executing interrupt. */
       // io_printf("R0: 0x%08x" CRLF, R0);
        io_printf("main: 0x%08p" CRLF, main);
       	io_printf("SysTickIntHandler: 0x%08p" CRLF, SysTickIntHandler);
       // io_printf("IntDefaultHandler: 0x%08p" CRLF, IntDefaultHandler);
       	print_vector_table();
        
        res = true;
    } else {
        LOG_ERROR(SYS, "Usage: cd ");
    }
    return res;
}
