#ifndef UNIT_TEST_H_
#define UNIT_TEST_H_

#include "unit_test_info.h"
#include <stdint.h>

void unit_test_init(void);
uint32_t get_unit_test_count(void);
const unit_test_info_t* get_unit_test(uint32_t test_index);
void dump_unit_test_all(void);
void dump_unit_test_key(const char* key);
bool unit_tests_run(const char* key);

#endif /* UNIT_TEST_H_ */
