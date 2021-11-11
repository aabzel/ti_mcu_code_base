#ifndef TEST_CLI_H
#define TEST_CLI_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

bool test_cli1(void);
bool test_cli2(void);

#define TEST_SUIT_CLI \
          {"cli1", test_cli1}, \
          {"cli2", test_cli2},

#ifdef __cplusplus
}
#endif

#endif /* TEST_CLI_H */
