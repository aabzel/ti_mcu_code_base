#ifndef TEST_CALENDAR_H
#define TEST_CALENDAR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#if 1
bool test_calendar_set_get_all(void);
#define TEST_SUIT_CALENDAR_EXT {"CalendarSetGetAll", test_calendar_set_get_all},
#else
#define TEST_SUIT_CALENDAR_EXT
#endif

bool test_calendar_parse_time(void);
bool test_calendar_set_get(void);
bool test_calendar_set_get_all(void);

#define TEST_SUIT_CALENDAR                                                                                             \
    {"CalendarSetGet", test_calendar_set_get}, {"CalendarParseTime", test_calendar_parse_time}, TEST_SUIT_CALENDAR_EXT

#ifdef __cplusplus
}
#endif

#endif /* TEST_CALENDAR_H */
