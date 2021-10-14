#ifndef TEST_CLI_H
#define TEST_CLI_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

bool test_cli(void);

#define TEST_SUIT_CLI \
          {"cli", test_cli},

#ifdef __cplusplus
}
#endif

#endif /* TEST_CLI_H */
