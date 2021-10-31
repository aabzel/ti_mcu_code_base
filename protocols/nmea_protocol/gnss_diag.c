#include "gnss_diag.h"
// http://traveleu.ru/map/GPSconverter.htm
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#include "gnss_utils.h"
#include "io_utils.h"
#include "log.h"

bool print_time_date(struct tm* time_date) {
    bool res = false;
    if(NULL != time_date) {
        io_printf("time: %02u:%02u:%02u" CRLF, time_date->tm_hour, time_date->tm_min, time_date->tm_sec);
        io_printf("date: %u/%u/%u" CRLF, time_date->tm_mday, time_date->tm_mon, time_date->tm_year);
    }
    return res;
}

bool print_coordinate(GnssCoordinate_t coordinate) {
    bool res = false;

    io_printf("latitude : %f " CRLF, coordinate.latitude);
    io_printf("longitude: %f " CRLF, coordinate.longitude);
    io_printf(" %f %f" CRLF, coordinate.latitude, coordinate.longitude);
    res = true;

    return res;
}
