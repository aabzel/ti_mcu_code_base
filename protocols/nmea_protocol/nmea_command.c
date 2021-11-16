#include "nmea_command.h"

#include <stdbool.h>
#include <stdio.h>

#include "convert.h"
#include "data_utils.h"
#include "gnss_diag.h"
#include "io_utils.h"
#include "log.h"
#include "table_utils.h"
#include "nmea_protocol.h"
#include "writer_generic.h"

static bool nmea_diag(void) {
    io_printf("msg_cnt     : %u" CRLF, NmeaProto.msg_cnt);
    io_printf("crc read cnt: %u" CRLF, NmeaProto.crc_read_cnt);
    io_printf("crc ok cnt  : %u" CRLF, NmeaProto.crc_ok_cnt);
    io_printf("crc err cnt : %u" CRLF, NmeaProto.crc_err_cnt);
    io_printf("proc_msg_cnt: %u" CRLF, NmeaProto.proc_msg_cnt);
    io_printf("err cnt : %u" CRLF, NmeaProto.err_cnt);
    io_printf("msg: [%s]" CRLF, NmeaProto.fix_message);
    return true;
}


static bool nmea_data(void) {
    print_time_date(&NmeaData.rmc.time_date);
    io_printf("RCM" CRLF);
    print_coordinate(NmeaData.rmc.coordinate_ddmm, true);
    NmeaData.rmc.coordinate_dd = encode_gnss_coordinates(NmeaData.rmc.coordinate_ddmm);
    print_coordinate(NmeaData.rmc.coordinate_dd, true);

    io_printf("GGA" CRLF);
    print_coordinate(NmeaData.gga.coordinate_ddmm, true);
    NmeaData.gga.coordinate_dd = encode_gnss_coordinates(NmeaData.gga.coordinate_ddmm);
    print_coordinate(NmeaData.gga.coordinate_dd, true);

    io_printf("speed_knots : %f" CRLF, NmeaData.rmc.speed_knots);
    io_printf("height: %f" CRLF, NmeaData.gga.height);
    io_printf("nb_sat: %u" CRLF, NmeaData.gga.nb_sat);

    io_printf("General" CRLF);
    print_coordinate(NmeaData.coordinate_dd, true);
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
