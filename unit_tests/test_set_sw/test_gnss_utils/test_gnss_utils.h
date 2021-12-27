#ifndef TEST_GNSS_UTILS_H
#define TEST_GNSS_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

bool test_gnss_utils(void);
bool test_gnss_azimuth(void);

#define TEST_SUIT_UTILS_GNSS             \
    {"gnss_utils", test_gnss_utils},     \
	{"gnss_azimuth", test_gnss_azimuth},

#ifdef __cplusplus
}
#endif

#endif /* TEST_GNSS_UTILS_H */
