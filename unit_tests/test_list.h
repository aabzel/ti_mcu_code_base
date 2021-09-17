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


#ifdef __cplusplus
}
#endif

#endif /*TEST_LIST_H*/
