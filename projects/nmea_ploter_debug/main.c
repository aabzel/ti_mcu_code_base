#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "array.h"
#include "macro_utils.h"
#include "nmea_plot.h"
#include "log.h"
#include "nmea_protocol.h"
#include "utils_file.h"
#include "win_utils.h"

#define VERSION 2

int main(int argc, char* argv[]) {
    bool res = false;
    int ret = 0;
    win_color_enable();
    set_log_level(SYS, LOG_LEVEL_DEBUG);
    LOG_INFO(SYS,"NmeaPlotter:");
    LOG_INFO(SYS,"Version: %u", VERSION);

    if(argc < 2) {
        LOG_ERROR(SYS," Lack of file");
        res = false;
        ret = 1;
    } else {
        LOG_INFO(SYS,"In file %s", argv[1]);
        LOG_INFO(SYS," Out file %s", argv[2]);
        res = true;
    }

    if(res) {
        nmea_init();
        res = plot_nmea(argv[1], argv[2]);
        if(false == res) {
            ret = 2;
        }
        system("plot_gnss_dots_from_file_m.py");
        system("plot_gnss_dots_from_file_deg.py");
    }

    return ret;
}
