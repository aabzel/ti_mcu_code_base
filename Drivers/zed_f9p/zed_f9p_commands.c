#include "zed_f9p_commands.h"

#include <inttypes.h>
#include <stdio.h>

#include "convert.h"
#include "gnss_diag.h"
#include "io_utils.h"
#include "log.h"
#include "zed_f9p_drv.h"

bool zed_f9p_diag_command(int32_t argc, char* argv[]){
    bool res = false;
    if(0 == argc) {
        io_printf("UTC %d" CRLF, ZedF9P.time_zone);
        res = print_time_date(&ZedF9P.time_date);
        io_printf("Current" CRLF);
        print_coordinate(ZedF9P.coordinate_cur);
        io_printf("Last" CRLF);
        print_coordinate(ZedF9P.coordinate_last);
        res = true;
    } else {
        LOG_ERROR(SYS, "Usage: zfd");
    }
    return res;
}

