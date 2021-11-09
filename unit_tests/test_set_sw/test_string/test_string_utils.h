#ifndef TEST_STRING_UTILS_H
#define TEST_STRING_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

bool test_count_substring(void);
bool test_string_utils(void);
bool test_string_utils_ext(void);
/*
tsr string_utils+
*/
#define TEST_SUIT_STRING_UTILS            \
  {"string_count_substring", test_count_substring},    \
  {"string_utils", test_string_utils},    \
  {"string_utils_ext", test_string_utils_ext},

  #ifdef __cplusplus
}
#endif

#endif /* TEST_STRING_UTILS_H */
