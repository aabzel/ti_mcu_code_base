#include "zed_f9p_diag.h"
 
#include <stdint.h>

#include "zed_f9p_drv.h"


char *RtkFixDur2str(uint32_t time_ms){
    static char name[30] = "";
    snprintf(name, sizeof(name), "%7.1fs=%7.1fmin", ((double)time_ms) / 1000.0,((double)time_ms) / 60000.0);
    return name;
}

const char *rtk_mode2str(uint8_t rtk_code) {
    const char *name ="undef";
    switch(rtk_code){
    case RTK_NONE:
        name ="Norm";
        break;
    case RTK_BASE:
        name ="Base";
        break;
    case RTK_ROVER:
        name ="Rover";
        break;
    default:
        name ="error";
        break;
    }
    return name;
}
