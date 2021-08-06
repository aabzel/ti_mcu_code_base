#ifndef TEST_LIST_H
#define TEST_LIST_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "unit_test_info.h"

extern const unit_test_info_t test_list[];

uint32_t get_test_list_cnt(void);

bool test_64bit_mult(void);
bool test_utoa_bin8(void);
bool test_float_to_uint16(void);
bool test_type_transformation(void);
bool test_array(void) ;

#ifdef __cplusplus
}
#endif

#endif /*TEST_LIST_H*/
