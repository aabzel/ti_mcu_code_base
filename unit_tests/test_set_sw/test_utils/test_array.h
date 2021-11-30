#ifndef TEST_ARRAY_H
#define TEST_ARRAY_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

bool test_array_con_pat(void);

#define TEST_SUIT_ARRAY {"array_con_pat", test_array_con_pat},

#ifdef __cplusplus
}
#endif

#endif /* TEST_ARRAY_H */
