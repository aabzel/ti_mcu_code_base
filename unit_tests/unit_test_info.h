#ifndef UNIT_TEST_INFO_H_
#define UNIT_TEST_INFO_H_

#include <stdbool.h>

typedef bool (*utest_funct_t)(void);

typedef struct {
    const char* const name;
    const utest_funct_t utest_func;
} unit_test_info_t;

#endif /* UNIT_TEST_INFO_H_ */
