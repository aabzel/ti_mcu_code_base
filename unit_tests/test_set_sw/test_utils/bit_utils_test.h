#ifndef TEST_BIT_UTILS_H
#define TEST_BIT_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

bool test_bit_mask(void);
bool test_bit_utils(void);
bool test_bit_type_size(void);
bool test_bit_macro(void);
/*
tsr bit_utils+
*/
#define TEST_SUIT_UTILS_BIT \
          {"bit_macro", test_bit_macro},\
          {"bit_utils", test_bit_utils},\
          {"bit_type_size", test_bit_type_size},\
          {"bit_mask", test_bit_mask},

#ifdef __cplusplus
}
#endif

#endif /* TEST_BIT_UTILS_H */
