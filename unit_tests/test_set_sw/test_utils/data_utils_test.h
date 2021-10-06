#ifndef TEST_DATA_UTILS_H
#define TEST_DATA_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

bool test_data_utils(void);

#define TEST_SUIT_UTILS_DATA {"data_utils", test_data_utils},

#ifdef __cplusplus
}
#endif

#endif /* TEST_DATA_UTILS_H */
