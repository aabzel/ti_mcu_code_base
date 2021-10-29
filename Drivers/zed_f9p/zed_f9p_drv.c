#include "zed_f9p_drv.h"

#include <stdbool.h>
#include <stdint.h>

#include "clocks.h"
#include "nmea_protocol.h"

extern ZedF9P_t ZedF9P={0};

bool zed_f9p_init(void){
    ZedF9P.is_init = true;
    return true;
}

bool zed_f9p_proc(void){
    /*is new GNSS sampless*/
    ZedF9P.time_date = NmeaData.rmc.time_date;
    ZedF9P.coordinate_cur = NmeaData.rmc.coordinate;
    ZedF9P.coordinate_last = ZedF9P.coordinate_cur ;
    return true;
}
