#include "health_monitor_command.h"

#include "health_monitor.h"

#include "convert.h"
#include "io_utils.h"
#include "log.h"
#include "table_utils.h"

bool health_monitor_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(0 == argc) {
        res = true;
    }
    if(1 <= argc) {
        res = try_str2bool(argv[0], &HealthMon.power);
        if(false == res) {
            LOG_ERROR(SYS, "Unable to extract power %s", argv[0]);
        }
    }
    io_printf("diag voltage %u" CRLF, HealthMon.power);
    io_printf("init error %u" CRLF, HealthMon.init_error);
    io_printf("crc error code %u" CRLF, HealthMon.crc_error_cnt);
    io_printf("error cnt %u" CRLF, HealthMon.error_cnt);
    return res;
}
