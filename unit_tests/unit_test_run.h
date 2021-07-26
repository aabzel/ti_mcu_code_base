#ifndef UNIT_TEST_RUN_H
#define UNIT_TEST_RUN_H

#include <stdbool.h>
#include <stdint.h>

extern bool test_failed;

uint32_t unit_test_run(uint32_t index);
void failed_tests_print(void);
void failed_tests_reset(void);

#endif /* UNIT_TEST_RUN_H */
