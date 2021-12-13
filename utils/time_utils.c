#include "time_utils.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#ifdef HAS_ZED_F9P
#include "zed_f9p_drv.h"
#endif /*HAS_ZED_F9P*/

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

struct tm* time_get_time(void) {
    struct tm* time = NULL;
#ifdef HAS_ZED_F9P
    time = &ZedF9P.time_date;
#endif
    return time;
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
            tm_stamp->tm_mon =
                extract_digit(packed_date, 3) * 10 + extract_digit(packed_date, 2) - 1; /*Ublox count from 1*/
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

bool is_time_date_equal(struct tm* date_time1, struct tm* date_time2) {
    bool res = true;
    if(res) {
        if(date_time1->tm_year == date_time2->tm_year) {
            res = true;
        } else {
            res = false;
        }
    }

    if(res) {
        if(date_time1->tm_mon == date_time2->tm_mon) {
            res = true;
        } else {
            res = false;
        }
    }
    if(res) {
        if(date_time1->tm_mday == date_time2->tm_mday) {
            res = true;
        } else {
            res = false;
        }
    }

    if(res) {
        if(date_time1->tm_hour == date_time2->tm_hour) {
            res = true;
        } else {
            res = false;
        }
    }

    if(res) {
        if(date_time1->tm_min == date_time2->tm_min) {
            res = true;
        } else {
            res = false;
        }
    }

    if(res) {
        if(date_time1->tm_sec == date_time2->tm_sec) {
            res = true;
        } else {
            res = false;
        }
    }

    return res;
}

bool is_valid_time_date(struct tm* date_time) {
    bool res = true;
    if(res) {
        if((0 <= date_time->tm_mon) && (date_time->tm_mon <= 11)) {
            res = true;
        } else {
            res = false;
        }
    }
    if(res) {
        if((1 <= date_time->tm_mday) && (date_time->tm_mday <= 31)) {
            res = true;
        } else {
            res = false;
        }
    }
    if(res) {
        if((0 <= date_time->tm_hour) && (date_time->tm_hour <= 23)) {
            res = true;
        } else {
            res = false;
        }
    }
    if(res) {
        if((0 <= date_time->tm_min) && (date_time->tm_min <= 59)) {
            res = true;
        } else {
            res = false;
        }
    }

    if(res) {
        if((0 <= date_time->tm_sec) && (date_time->tm_sec <= 59)) {
            res = true;
        } else {
            res = false;
        }
    }

    return res;
}

bool is_valid_time(struct tm* date_time) {
    bool res = true;
    if(res) {
        if((0 <= date_time->tm_hour) && (date_time->tm_hour <= 23)) {
            res = true;
        } else {
            res = false;
        }
    }
    if(res) {
        if((0 <= date_time->tm_min) && (date_time->tm_min <= 59)) {
            res = true;
        } else {
            res = false;
        }
    }

    if(res) {
        if((0 <= date_time->tm_sec) && (date_time->tm_sec <= 59)) {
            res = true;
        } else {
            res = false;
        }
    }

    return res;
}

bool time_get_time_str(char* str, uint32_t size) {
    bool res = false;
    if(str && (0 < size)) {
        struct tm* curTime = time_get_time();
        if(curTime) {
            res = is_valid_time(curTime);
            if(res) {
                snprintf(str, size, "%02u:%02u:%02u", curTime->tm_hour, curTime->tm_min, curTime->tm_sec);
            }
        } else {
            strncpy(str, "", size);
        }
        res = true;
    }
    return res;
}

bool parse_date_time_from_string(char* name, struct tm* tm_stamp) { return true; }
