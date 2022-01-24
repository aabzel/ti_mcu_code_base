
#include "tbfp_diag.h"

#include <string.h>
#include <time.h>
#include <inttypes.h>

#include "byte_utils.h"
#include "data_utils.h"
#include "debug_info.h"
#include "time_diag.h"
#include "gnss_diag.h"
#include "io_utils.h"
#include "tbfp_protocol.h"

bool tbfp_print_ping_frame(TbfPingFrame_t *pingFrame){
    bool res = true;
    if (pingFrame) {
        io_printf("MAC: 0x%" PRIX64 CRLF, pingFrame->mac);
        struct tm * time_date =  gmtime(&pingFrame->time_stamp);
        
        if (time_date) {
#ifdef HAS_GNSS
            res =  print_time_date(time_date )&&res;
#endif
        } else {
            res = false;
        }
#ifdef HAS_GNSS
        res = print_coordinate(pingFrame->coordinate, true)&&res;
#endif
    }else{
        res = false;
    }
    return res;
}
