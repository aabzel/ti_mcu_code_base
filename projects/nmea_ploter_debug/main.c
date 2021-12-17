#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "array.h"
#include "macro_utils.h"
#include "nmea_plot.h"
#include "nmea_protocol.h"
#include "utils_file.h"

#define VERSION 2

int main(int argc, char* argv[]) {
    bool res = false;
    int ret = 0;
    printf("\n[*] NmeaPlotter");
    printf("\n[*] Version: %u", VERSION);

    if(argc < 2) {
        printf("\n[e] Lack of file");
        res = false;
        ret = 1;
    } else {
        printf("\n[i] In file %s", argv[1]);
        printf("\n[i] Out file %s", argv[2]);
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
