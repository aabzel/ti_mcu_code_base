#ifndef TEST_BIT_UTILS_H
#define TEST_BIT_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

bool test_bit_utils(void);
/*
tsr bit_utils+
*/
#define BIT_UTILS_TEST_SUIT {"bit_utils", test_bit_utils},

#ifdef __cplusplus
}
#endif

#endif /* TEST_BIT_UTILS_H */
