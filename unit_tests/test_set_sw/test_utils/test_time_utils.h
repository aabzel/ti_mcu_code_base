#ifndef TEST_TIME_UTILS_H
#define TEST_TIME_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

bool test_time_utils(void);

#define TIME_UTILS_TEST_SUIT {"time_utils", test_time_utils},

#ifdef __cplusplus
}
#endif

#endif /* TEST_TIME_UTILS_H */
