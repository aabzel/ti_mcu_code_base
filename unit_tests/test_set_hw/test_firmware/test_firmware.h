#ifndef TEST_FIRMWARE_H
#define TEST_FIRMWARE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

bool test_interfaces(void);

#define TEST_SUIT_FIRMWARE                      \
        {"interfaces", test_interfaces},                                                                                                     \

#ifdef __cplusplus
}
#endif

#endif /* TEST_FIRMWARE_H */
