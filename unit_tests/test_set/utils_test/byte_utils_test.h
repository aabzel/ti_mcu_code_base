#ifndef TEST_BYTE_UTILS_H
#define TEST_BYTE_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

bool test_byte_utils(void);

#define BYTE_UTILS_TEST_SUIT {"byte_utils", test_byte_utils},

#ifdef __cplusplus
}
#endif

#endif /* TEST_BYTE_UTILS_H */
