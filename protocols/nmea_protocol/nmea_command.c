#include "nmea_command.h"

#include <stdbool.h>
#include <stdio.h>

#include "convert.h"
#include "data_utils.h"
#include "gnss_diag.h"
#include "io_utils.h"
#include "log.h"
#include "table_utils.h"
#include "nmea_diag.h"
#include "nmea_protocol.h"
#include "writer_config.h"
#include "writer_generic.h"

static bool nmea_diag(void) {
    io_printf("msg_cnt     : %u" CRLF, NmeaProto.msg_cnt);
    io_printf("crc read cnt: %u" CRLF, NmeaProto.crc_read_cnt);
    io_printf("crc ok cnt  : %u" CRLF, NmeaProto.crc_ok_cnt);
    io_printf("crc err cnt : %u" CRLF, NmeaProto.crc_err_cnt);
    io_printf("proc_msg_cnt: %u" CRLF, NmeaProto.proc_msg_cnt);
    io_printf("err cnt : %u" CRLF, NmeaProto.err_cnt);
    io_printf("undef messeges: %u" CRLF, NmeaProto.undef_err_cnt);
    io_printf("undef: %s"CRLF, NmeaProto.undef_message);
    io_printf("msg: [%s]" CRLF, NmeaProto.fix_message);
    return true;
}

static bool nmea_data(void) {
    print_time_date(&NmeaData.rmc.time_date);
    io_printf("RCM" CRLF);
#ifdef HAS_NMEA_DIAG
    io_printf("PosMode: %u %s" CRLF, NmeaProto.pos_mode, nmea_pos_mode2std(NmeaProto.pos_mode));
    io_printf("RTKFixed max duration: %u ms" CRLF, NmeaProto.rtk_fixed_max_duration_ms);
#endif
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

static bool nmea_messages(void){
    bool res = false;
    static const table_col_t cols[] = {{5, "mess"},
                                       {8, "hCnt"},
                                       {8, "pCnt"},
    };
    table_header(&(curWriterPtr->s), cols, ARRAY_SIZE(cols));
    io_printf(TSEP" GLL "TSEP" %6u "TSEP" %6u "TSEP CRLF, NmeaData.gll.fcnt.h_cnt,NmeaData.gll.fcnt.cnt);
    io_printf(TSEP" RMC "TSEP" %6u "TSEP" %6u "TSEP CRLF, NmeaData.rmc.fcnt.h_cnt,NmeaData.rmc.fcnt.cnt);
    io_printf(TSEP" VTG "TSEP" %6u "TSEP" %6u "TSEP CRLF, NmeaData.vtg.fcnt.h_cnt,NmeaData.vtg.fcnt.cnt);
    io_printf(TSEP" GGA "TSEP" %6u "TSEP" %6u "TSEP CRLF, NmeaData.gga.fcnt.h_cnt,NmeaData.gga.fcnt.cnt);
    io_printf(TSEP" GSV "TSEP" %6u "TSEP" %6u "TSEP CRLF, NmeaData.gsv.fcnt.h_cnt,NmeaData.gsv.fcnt.cnt);
    io_printf(TSEP" PBUX"TSEP" %6u "TSEP" %6u "TSEP CRLF,NmeaData.pbux.fcnt.h_cnt,NmeaData.pbux.fcnt.cnt);
    io_printf(TSEP" ZDA "TSEP" %6u "TSEP" %6u "TSEP CRLF, NmeaData.zda.fcnt.h_cnt,NmeaData.zda.fcnt.cnt);
    table_row_bottom(&(curWriterPtr->s), cols, ARRAY_SIZE(cols));

    return res;
}

bool nmea_messages_command(int32_t argc, char* argv[]){
    bool res = false;
    if(0 == argc) {
        res = nmea_messages();
    } else {
        LOG_ERROR(SYS, "Usage: nmm");
    }
    return res;
}
