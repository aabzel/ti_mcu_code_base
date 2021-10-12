#ifndef TEST_STRING_UTILS_H
#define TEST_STRING_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

bool test_string_utils(void);
/*
tsr string_utils+
*/
#define TEST_SUIT_STRING_UTILS {"string_utils", test_string_utils},

#ifdef __cplusplus
}
#endif

#endif /* TEST_STRING_UTILS_H */
