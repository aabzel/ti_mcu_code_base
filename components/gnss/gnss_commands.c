#include "gnss_commands.h"

#include <stdbool.h>
#include <stdint.h>

#include "io_utils.h"
#include "log.h"
#include "time_diag.h"
#include "gnss_diag.h"
#include "gnss_drv.h"

bool gnss_data_command(int32_t argc, char* argv[]){
    bool res = false;
    res=print_time_date(&Gnss.time_date);
    res=print_coordinate(Gnss.coordinate_cur, true);
    return res;
}

