
#include "time_utils.h"

#include <stdbool.h>
#include <time.h>

#include "data_utils.h"
// UTC hour in hhmmss format
bool parse_time_from_val(uint32_t packed_time, struct tm* tm_stamp) {
    bool res = false;
    if(tm_stamp) {
        res = true;
        tm_stamp->tm_sec = extract_digit(packed_time, 1) * 10 + extract_digit(packed_time, 0);
        if(60 <= tm_stamp->tm_sec) {
            res = false;
        }
        if(true == res) {
            tm_stamp->tm_min = extract_digit(packed_time, 3) * 10 + extract_digit(packed_time, 2);
            if(60 <= tm_stamp->tm_min) {
                res = false;
            }
        }

        if(true == res) {
            tm_stamp->tm_hour = extract_digit(packed_time, 5) * 10 + extract_digit(packed_time, 4);
            if(24 <= tm_stamp->tm_hour) {
                res = false;
            }
        }
    }
    return res;
}

/* date in ddmmaa format */
bool parse_date_from_val(uint32_t packed_date, struct tm* tm_stamp) {
    bool res = false;
    if(tm_stamp) {
        res = true;
        tm_stamp->tm_mday = extract_digit(packed_date, 5) * 10 + extract_digit(packed_date, 4);
        if(32 <= tm_stamp->tm_mday) {
            res = false;
        }
        if(true == res) {
            tm_stamp->tm_mon = extract_digit(packed_date, 3) * 10 + extract_digit(packed_date, 2);
            if(13 <= tm_stamp->tm_mon) {
                res = false;
            }
        }

        if(true == res) {
            tm_stamp->tm_year = extract_digit(packed_date, 1) * 10 + extract_digit(packed_date, 0);
            tm_stamp->tm_year += 2000;
        }
    }
    return res;
}
