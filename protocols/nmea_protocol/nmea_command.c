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

static bool print_time_date(struct tm *time_date){
    bool res = false;
    if(NULL!=time_date){
        io_printf("time: %02u:%02u:%02u" CRLF, time_date->tm_hour, time_date->tm_min, time_date->tm_sec);
        io_printf("date: %u/%u/%u" CRLF, time_date->tm_mday, time_date->tm_mon, time_date->tm_year);
    }
    return res;
}

static bool nmea_data(void) {
    io_printf("utc: %u" CRLF, NmeaData.rmc.utc);
    io_printf("date: %u" CRLF, NmeaData.rmc.date);
    print_time_date( &NmeaData.rmc.time_date);
    io_printf(" %f %f" CRLF, NmeaData.rmc.lat , NmeaData.rmc.lon);
    io_printf(" %f %f" CRLF, NmeaData.gga.lat , NmeaData.gga.lon);
    io_printf("speed_knots : %f" CRLF, NmeaData.rmc.speed_knots);
    io_printf("height: %f" CRLF, NmeaData.gga.height);
    io_printf("nb_sat: %u" CRLF, NmeaData.gga.nb_sat);
    return true;
}

bool nmea_stat_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(0 == argc) {
        res = nmea_diag();
    } else {
        LOG_ERROR(SYS, "Usage: nms");
    }
    return res;
}

bool nmea_data_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(0 == argc) {
        res = nmea_data();
    } else {
        LOG_ERROR(SYS, "Usage: nmd");
    }
    return res;
}
