#ifndef TEST_TIME_UTILS_H
#define TEST_TIME_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

bool test_time_utils(void);
bool test_time_cmp(void);

#define TEST_SUIT_UTILS_TIME  \
    {"time_utils", test_time_utils}, \
    {"time_cmp", test_time_cmp},

#ifdef __cplusplus
}
#endif

#endif /* TEST_TIME_UTILS_H */
