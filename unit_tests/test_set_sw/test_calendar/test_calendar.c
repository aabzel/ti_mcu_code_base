#include "test_calendar.h"

#ifdef X86_64
#include <stdio.h>
#endif

#include "calendar.h"
#include "time_utils.h"
#include "unit_test_check.h"

bool test_calendar_parse_time(void) {
    struct tm date_time;
    EXPECT_TRUE(time_data_parse(&date_time, "Tue Dec  7 15:34:46 2021"));
    EXPECT_EQ(46, date_time.tm_sec);
    EXPECT_EQ(34, date_time.tm_min);
    EXPECT_EQ(15, date_time.tm_hour);
    EXPECT_EQ(7, date_time.tm_mday);
    EXPECT_EQ(11, date_time.tm_mon);
    EXPECT_EQ(2021, date_time.tm_year);
    return true;
}

bool test_calendar_set_get(void) {
#ifdef X86_64
    printf("\n%s()", __FUNCTION__);
#endif
    struct tm date_time_set;
    struct tm date_time_get = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    date_time_set.tm_sec = 11;
    date_time_set.tm_min = 57;
    date_time_set.tm_hour = 19;
    date_time_set.tm_mday = 17;
    date_time_set.tm_mon = 11;
    date_time_set.tm_year = 2021;

    calendar_settime(&date_time_set);

    EXPECT_TRUE(calendar_gettime(&date_time_get));
    EXPECT_EQ(11, date_time_get.tm_sec);
    EXPECT_EQ(57, date_time_get.tm_min);
    EXPECT_EQ(19, date_time_get.tm_hour);
    EXPECT_EQ(17, date_time_get.tm_mday);
    EXPECT_EQ(11, date_time_get.tm_mon);
    EXPECT_EQ(2021, date_time_get.tm_year);
    return true;
}
