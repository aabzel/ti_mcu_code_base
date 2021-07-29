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
    io_printf("pos         : %u" CRLF, NmeaProto.pos);
    io_printf("proc_msg_cnt: %u" CRLF, NmeaProto.proc_msg_cnt);
    io_printf("msg: [%s]" CRLF, NmeaProto.fix_message);
    return true;
}
 
bool nmea_diag_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(0 == argc) {
        res = nmea_diag();
    } else {
        LOG_ERROR(SYS, "Usage: nmd");
    }
    return res;
}
 
