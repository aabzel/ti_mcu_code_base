#include "nmea_command.h"

#include <stdbool.h>
#include <stdio.h>

#include "convert.h"
#include "data_utils.h"
#include "io_utils.h"
#include "log.h"
#include "table_utils.h"
#include "nmea_protocol.h"
#include "writer_generic.h"

static bool nmea_diag(void) {
    io_printf("msg_cnt     : %u" CRLF, NmeaProto.msg_cnt);
    io_printf("crc err cnt : %u" CRLF, NmeaProto.crc_err_cnt);
    io_printf("crc ok cnt  : %u" CRLF, NmeaProto.crc_ok_cnt);
    io_printf("proc_msg_cnt: %u" CRLF, NmeaProto.proc_msg_cnt);
    io_printf("msg: [%s]" CRLF, NmeaProto.fix_message);
    return true;
}
 
static bool nmea_data(void) {
    io_printf("utc: %u" CRLF, NmeaData.rmc.utc);
    io_printf("data     : %u" CRLF, NmeaData.rmc.date);
    io_printf("%f %f" CRLF, NmeaData.rmc.lat , NmeaData.rmc.lon);
    io_printf("speed_knots : %d" CRLF, NmeaData.rmc.speed_knots);
    return true;
}

bool nmea_stat_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(0 == argc) {
        res = nmea_diag();
    } else {
        LOG_ERROR(SYS, "Usage: nmm");
    }
    return res;
}


bool nmea_data_command(int32_t argc, char* argv[]){
    bool res = false;
    if(0 == argc) {
        res = nmea_data();
    } else {
        LOG_ERROR(SYS, "Usage: nmd");
    }
    return res;
}
