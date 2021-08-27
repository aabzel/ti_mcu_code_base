#ifndef TEST_SX1262_H
#define TEST_SX1262_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

bool test_sx1262(void);

#define TEST_SUIT_SX1262 {"sx1262", test_sx1262},

#ifdef __cplusplus
}
#endif

#endif /* TEST_SX1262_H */
