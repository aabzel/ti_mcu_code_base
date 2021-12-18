#ifndef TEST_CALENDAR_H
#define TEST_CALENDAR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

bool test_calendar_parse_time(void);
bool test_calendar_set_get(void);

#define TEST_SUIT_CALENDAR  \
{"CalendarParseTime", test_calendar_parse_time},\
{"CalendarSetGet", test_calendar_set_get},

#ifdef __cplusplus
}
#endif

#endif /* TEST_CALENDAR_H */
