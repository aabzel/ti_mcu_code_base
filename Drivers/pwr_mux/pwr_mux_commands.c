#include "pwr_mux_commands.h"

#include <inttypes.h>

#include "convert.h"
#include "gpio_drv.h"
#include "log.h"
#include "io_utils.h"
#include "pwr_mux_diag.h"
#include "pwr_mux_drv.h"
#include "sys_config.h"
#include "task_info.h"
#ifdef HAS_TCAN4550
#include "tcan4550_drv.h"
#endif
 
bool pwr_mux_set_command(int32_t argc, char* argv[]){
    bool res = false;
    uint8_t source = 0;
    if(1 == argc) {
        res = try_str2uint8(argv[0], &source);
        if(false == res) {
            LOG_ERROR(LG_PWR, "Unable to parse source", argv[0]);
        }
        if(res){
          res= pwr_src_set((PwrSource_t)source);
          if(true==res){
              LOG_INFO(LG_PWR, "OK");
          }else{
              LOG_ERROR(LG_PWR, "Error");
          }
        }
    } else {
        LOG_ERROR(LG_PWR, "Usage: pms source");
        LOG_INFO(LG_PWR, "source [0-VCC_3V3 1-PWR_SRC_3V0_BATT]");
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
        LOG_ERROR(LG_PWR, "Usage: pmd");
    }
    return res;
}

bool pwr_set_save_mode_command(int32_t argc, char *argv[]) {
    bool res = false;
    bool state = false;
    if(1 == argc) {
        res = try_str2bool(argv[0], &state);
        if(false == res) {
            LOG_ERROR(LG_PWR, "Unable to extract state %s", argv[0]);
        }
    }
    if(res){
        if (true==state) {
            LOG_INFO(LG_PWR, "Enter power save mode");
#ifdef HAS_TCAN4550
            res = tcan4550_deinit() && res;
            task_data[TASK_ID_TCAN4550].on = false;
#endif
            res = gpio_set_state(DIO_PS_RS232 , 0)&& res;
        } else {
            LOG_INFO(LG_PWR, "Enter power normal mode");
            res = gpio_set_state(DIO_PS_RS232 , 1);
#ifdef HAS_TCAN4550
            task_data[TASK_ID_TCAN4550].on = true;
#endif
        }
    }

    return res;
}


