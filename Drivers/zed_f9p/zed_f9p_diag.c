#include "zed_f9p_diag.h"
 
#include <stdint.h>

#include "zed_f9p_drv.h"

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
