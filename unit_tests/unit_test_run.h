#ifndef UNIT_TEST_RUN_H
#define UNIT_TEST_RUN_H

#include <stdbool.h>
#include <stdint.h>

extern bool test_failed;

bool unit_test_run(uint32_t index);
#ifdef HAS_CLI
void failed_tests_print(void);
#endif
void failed_tests_reset(void);

#endif /* UNIT_TEST_RUN_H */
