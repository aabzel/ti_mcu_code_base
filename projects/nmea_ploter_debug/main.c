#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "array.h"
#include "macro_utils.h"
#include "utils_file.h"
#include "nmea_plot.h"
#include "nmea_protocol.h"

#define VERSION 1

int main(int argc, char* argv[]) {
    bool res = false;
	int ret = 0;
    printf("\n[*] NmeaPlotter");
    printf("\n[*] Version: %u", VERSION);

    if(argc < 1) {
        printf("\n[e] Lack of file");
        res = false;
		ret = 1;
    } else {
        printf("\n[e] file",argv[1]);
        res = true;
    }

    if(res) {
		nmea_init();
		res = plot_nmea(argv[1]);
		if(false==res){
			ret = 2;
		}
        system("plot_gnss_dots_from_file.py");
    }
	

    return ret;
}
