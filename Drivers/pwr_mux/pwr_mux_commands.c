#include "pwr_mux_commands.h"

#include <inttypes.h>

#include "convert.h"
#include "pwr_mux_drv.h"
#include "log.h"
#include "io_utils.h"
 
bool pwr_mux_set_command(int32_t argc, char* argv[]){
    bool res = false;
    uint8_t source = 0;
    if(1 == argc) {
        res = try_str2uint8(argv[0], &source);
        if(false == res) {
            LOG_ERROR(PWR, "Unable to parse source", argv[0]);
        }
        if(res){
          res= pwr_src_set((PwrSource_t)source);
          if(true==res){
              LOG_INFO(PWR, "OK");
          }else{
              LOG_ERROR(PWR, "Error");
          }
        }
    } else {
        LOG_ERROR(PWR, "Usage: pms source");
        LOG_INFO(PWR, "source [0-VCC_3V3 1-PWR_SRC_3V0_BATT]");
    }
    return res;
}

bool pwr_mux_diag_command(int32_t argc, char* argv[]){
    bool res = false;
    if(0 == argc) {
        res = true;
        PwrSource_t pwr_source=PWR_SRC_UNDEF;
        pwr_source = pwr_src_get();
        io_printf("power source %u %s"CRLF, pwr_source, pwr_source2str(pwr_source));
    } else {
        LOG_ERROR(PWR, "Usage: pmd");
    }
    return res;
}



