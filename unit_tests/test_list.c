#include "test_list.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>

#include "clocks.h"
#include "convert.h"
#include "debug_info.h"
#include "data_utils.h"
#include "diag_sys.h"
#include "io_utils.h"
#include "test_hw_list.h"
#include "test_sw_list.h"
#include "unit_test_check.h"
#include "writer_generic.h"

/*Compile time assemble array */
const unit_test_info_t test_list[] = {
    TEST_SUIT_HW
	TEST_SUIT_SW
};

uint32_t get_test_list_cnt(void) {
    uint32_t cnt = 0;
    cnt = ARRAY_SIZE(test_list);
    return cnt;
}




