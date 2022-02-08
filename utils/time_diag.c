#include "time_diag.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "io_utils.h"

bool print_time_date(struct tm* time_date) {
    bool res = false;
    if(NULL != time_date) {
        io_printf("time: %02u:%02u:%02u" CRLF, time_date->tm_hour, time_date->tm_min, time_date->tm_sec);
        io_printf("date: %u/%u/%u" CRLF, time_date->tm_mday, time_date->tm_mon+1, time_date->tm_year);
        res = true;
    }
    return res;
}

bool print_time(struct tm* time_date) {
    bool res = false;
    if(NULL != time_date) {
        io_printf("time: %02u:%02u:%02u" CRLF, time_date->tm_hour, time_date->tm_min, time_date->tm_sec);
        res = true;
    }
    return res;
}

