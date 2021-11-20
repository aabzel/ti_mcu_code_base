#ifndef TEST_SYSTEM_H
#define TEST_SYSTEM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

bool test_rtcm3_path(void);

#define TEST_SYSTEM                      \
        {"rtcm3_path", test_rtcm3_path},                                                                                                     \

#ifdef __cplusplus
}
#endif

#endif /* TEST_SYSTEM_H */
