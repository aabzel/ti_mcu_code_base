#ifndef TEST_TIME_UTILS_H
#define TEST_TIME_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

bool test_time_utils(void);

#define TEST_SUIT_UTILS_TIME {"time_utils", test_time_utils},

#ifdef __cplusplus
}
#endif

#endif /* TEST_TIME_UTILS_H */
