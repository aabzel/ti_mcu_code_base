#include "calendar.h"

#include <stdbool.h>
#include <time.h>

#include "log.h"
#ifdef HAS_MCU
#include "time_diag.h"
#endif
#include "time_utils.h"
#ifdef HAS_RTC
#include "rtc_drv.h"
#endif

#define FIRSTYEAR 2000 // start year
#define FIRSTDAY 6     // 0 = Sunday

static uint32_t g_sec;

static const uint8_t DaysInMonth[] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
/*
 * Function Name  : counter_to_struct
 * Description    : populates time-struct based on counter-value
 * Input          : - counter-value (unit seconds, 0 -> 1.1.2000 00:00:00),
 *                  - Pointer to time-struct
 * Output         : time-struct gets populated, DST not taken into account here
 * Return         : none
 */
static bool counter_to_struct(uint32_t sec, struct tm* t) {
    bool res = false;
    if(t) {
        uint16_t day = 0;
        uint8_t year = 0;
        uint16_t dayofyear = 0;
        uint8_t leap400 = 0;
        uint8_t month = 0;

        t->tm_sec = sec % 60;
        sec /= 60;
        t->tm_min = sec % 60;
        sec /= 60;
        t->tm_hour = sec % 24;
        day = (uint16_t)(sec / 24);

        t->tm_wday = (day + FIRSTDAY) % 7; // weekday

        year = FIRSTYEAR % 100;                    // 0..99
        leap400 = 4 - ((FIRSTYEAR - 1) / 100 & 3); // 4, 3, 2, 1

        for(;;) {
            dayofyear = 365;
            if(0 == (year & 3)) {
                dayofyear = 366;                                    // leap year
                if((0 == year) || (100 == year) || (200 == year)) { // 100 year exception
                    if(--leap400) {                                 // 400 year exception
                        dayofyear = 365;
                    }
                }
            }
            if(day < dayofyear) {
                break;
            }
            day -= dayofyear;
            year++; // 00..136 / 99..235
        }
        t->tm_year = year + FIRSTYEAR / 100 * 100; // + century

        if((dayofyear & 1) && (58 < day)) { // no leap year and after 28.2.
            day++;                          // skip 29.2.
        }

        for(month = 1; DaysInMonth[month - 1] <= day; month++) {
            day -= DaysInMonth[month - 1];
        }

        t->tm_mon = month-1;    // 0..11
        t->tm_mday = day + 1; // 1..31
        res = true;
    }
    return res;
}

bool calendar_gettime(struct tm* date_time) {
    uint32_t cur_sec = 0;
    cur_sec = g_sec; /*For tests on PC */
#ifdef HAS_RTC
    cur_sec = SwRtc.raw_sec;
#endif
    counter_to_struct(cur_sec, date_time); // get non DST time
    return true;
}

/*
 * Function Name  : struct_to_counter
 * Description    : calculates second-counter from populated time-struct
 * Input          : Pointer to time-struct
 * Output         : none
 * Return         : counter-value (unit seconds, 0 -> 1.1.2000 00:00:00),
 */
static uint32_t struct_to_counter(struct tm* t) {
    uint8_t i = 0;
    uint32_t counter = 0;
    uint16_t idx = 0, year = 0;

    year = t->tm_year;

    /* Calculate days of years before */
    counter = (uint32_t)year * 365;
    if(1 <= t->tm_year) {
        counter += (year + 3) / 4;
        counter -= (year - 1) / 100;
        counter += (year - 1) / 400;
    }

    /* Start with 2000 a.d. */
    counter -= 730485UL;

    /* Make month an array index */
    idx = t->tm_mon;

    /* Loop thru each month, adding the days */
    for(i = 0; i < idx; i++) {
        counter += DaysInMonth[i];
    }

    /* Leap year? adjust February */
    if((year % 400) == 0 || ((year % 4) == 0 && (year % 100) != 0)) {
        LOG_DEBUG(LG_CAL,"leap year:%u",year);
    } else {
        if(1 < t->tm_mon) {
            counter--;
        }
    }

    /* Add remaining days */
    counter += t->tm_mday;

    /* Convert to seconds, add all the other stuff */
    counter = (counter - 1) * 86400L + (uint32_t)t->tm_hour * 3600 + (uint32_t)t->tm_min * 60 + t->tm_sec;
    LOG_DEBUG(LG_CAL,"counter:%u=0x%x",counter,counter);
    return counter;
}

uint32_t calendar_settime(struct tm* date_time) {
    bool res =  false;
#ifdef HAS_RTC
    print_time_date(date_time);
#endif
    res = is_valid_time_date(date_time);
    if(res){
      LOG_DEBUG(LG_CAL, "valid date time");
      g_sec = struct_to_counter(date_time);
#ifdef HAS_RTC
      SwRtc.raw_sec = g_sec;
#endif
    }

    return g_sec;
}

bool calendar_init(void) {
    bool res = false;
    struct tm date_time;
#if 0
    res = time_date_parse(&date_time, __TIMESTAMP__);
    if(res) {
        calendar_settime(&date_time);
    }
#endif
    res = time_parse(&date_time, __TIME__);
    if(res) {
        calendar_settime(&date_time);
    }else{
        LOG_ERROR(LG_CAL, "TimeParseErr");
    }
    res = date_parse(&date_time, __DATE__);
    if(res) {
        calendar_settime(&date_time);
    }else{
        LOG_ERROR(LG_CAL, "DateParseErr");
    }
    return res;
}
