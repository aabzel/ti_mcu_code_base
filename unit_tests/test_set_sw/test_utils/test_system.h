#ifndef TEST_SYSTEM_H
#define TEST_SYSTEM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

bool test_system(void);

#define TEST_SUIT_SYSTEM_SW {"system", test_system},

#ifdef __cplusplus
}
#endif

#endif /* TEST_SYSTEM_H */
