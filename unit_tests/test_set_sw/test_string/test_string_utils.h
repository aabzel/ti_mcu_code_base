#ifndef TEST_STRING_UTILS_H
#define TEST_STRING_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

bool test_replaic_char(void) ;

#define TEST_SUIT_STRING_UTILS_BASE            \
  {"str_replaic_char", test_replaic_char},


#ifdef X86_64
bool test_string_utils_ext(void);
bool test_count_substring(void);
#define TEST_SUIT_STRING_UTILS_EXT         \
  {"str_count_substring", test_count_substring},    \
  {"str_utils_ext", test_string_utils_ext},
#else
#define TEST_SUIT_STRING_UTILS_EXT
#endif

#define TEST_SUIT_STRING_UTILS       \
        TEST_SUIT_STRING_UTILS_EXT   \
        TEST_SUIT_STRING_UTILS_BASE

  #ifdef __cplusplus
}
#endif

#endif /* TEST_STRING_UTILS_H */
