#ifndef TEST_FLOAT_UTILS_H
#define TEST_FLOAT_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

bool test_float_utils(void);

#define FLOAT_UTILS_TEST_SUIT {"float_utils", test_float_utils},

#ifdef __cplusplus
}
#endif

#endif /* TEST_FLOAT_UTILS_H */
