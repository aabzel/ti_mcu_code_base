#include "system_commands.h"

#include "log.h"
#include "system.h"

bool sys_bypass_nmea_rs232_command(int32_t argc, char* argv[]){
    bool res = false;
    if (0 == argc) {
#ifdef HAS_HARVESTER_V1
        res = sys_bypass_nmea_rs232();
#endif
        if(res){
            LOG_INFO(SYS, "Ok!");
        }
    } else {
        LOG_ERROR(SYS, "Usage: nmrs");
    }
    return res;
}
