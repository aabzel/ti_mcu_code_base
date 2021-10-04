
#include "rtcm3_command.h"

#include <stdbool.h>
#include <stdio.h>

#include "convert.h"
#include "data_utils.h"
#include "io_utils.h"
#include "log.h"
#include "table_utils.h"
#include "rtcm3_protocol.h"
#include "writer_generic.h"

static bool rtcm3_diag(void) {
    io_printf("rx pkt cnt : %u" CRLF, Rtcm3Porotocol.rx_pkt_cnt);
    io_printf("crc err cnt : %u" CRLF, Rtcm3Porotocol.crc_err_cnt);
    return true;
}

bool rtcm3_diag_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(0 == argc) {
        res = rtcm3_diag();
    } else {
        LOG_ERROR(SYS, "Usage: rtcm");
    }
    return res;
}

bool rtcm3_reset_command(int32_t argc, char* argv[]){
    bool res = false;
    if(0 == argc) {
        res = rtcm3_reset_rx();
    } else {
        LOG_ERROR(SYS, "Usage: rtcmr");
    }
    return res;
}
