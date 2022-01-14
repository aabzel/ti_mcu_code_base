#include "test_list.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>

#include "convert.h"
#include "debug_info.h"
#include "data_utils.h"
#include "io_utils.h"
#ifdef HAS_HW_TESTS
#include "clocks.h"
#include "test_hw_list.h"
#endif /*HAS_HW_TESTS*/
#ifdef HAS_SW_TESTS
#include "test_sw_list.h"
#endif /*HAS_SW_TESTS*/
#include "unit_test_check.h"
#include "writer_generic.h"

/*Compile time assemble array */
const unit_test_info_t test_list[] = {
#ifdef HAS_HW_TESTS
    TEST_SUIT_HW
#endif /*HAS_HW_TESTS*/
#ifdef HAS_SW_TESTS
	TEST_SUIT_SW
#endif /*HAS_SW_TESTS*/
};

uint32_t get_test_list_cnt(void) {
    uint32_t cnt = 0;
    cnt = ARRAY_SIZE(test_list);
    return cnt;
}
