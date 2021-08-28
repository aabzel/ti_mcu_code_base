#ifndef TEST_BYTE_UTILS_H
#define TEST_BYTE_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

bool test_copy_rev(void);
bool test_byte_reverse(void);

#define BYTE_UTILS_TEST_SUIT \
                             {"byte_reverse", test_byte_reverse}, \
                             {"copy_rev", test_copy_rev},

#ifdef __cplusplus
}
#endif

#endif /* TEST_BYTE_UTILS_H */
