#include "test_hw_list.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>

#include "clocks.h"
#include "debug_info.h"
#include "io_utils.h"
#include "none_blocking_pause.h"
#include "unit_test_check.h"
#include "writer_generic.h"

bool test_clock_us(void) {
    uint32_t i = 0;
    uint64_t up_time_us_cur = 0;
    uint64_t up_time_us_prev = 0;
    wait_in_loop_ms(20) ;
    up_time_us_prev = get_time_us();
    pause_1us();
    for(i = 0; i < 10000; i++) {
        up_time_us_cur = get_time_us();
        EXPECT_GR_64(up_time_us_prev, up_time_us_cur, i);
        pause_1us();
        pause_1us();
        up_time_us_prev = up_time_us_cur;
    }
    return true;
}

bool test_clock_ms(void) {
    uint32_t i = 0;
    uint32_t up_time_ms_cur = 0;
    uint32_t up_time_ms_prev = 0;
    up_time_ms_prev = get_time_ms32();
    pause_1ms();
    for(i = 0; i < 500; i++) {
        up_time_ms_cur = get_time_ms32();
        EXPECT_GR_64(up_time_ms_prev, up_time_ms_cur, i);
        pause_1ms();
        pause_1ms();
        up_time_ms_prev = up_time_ms_cur;
    }
    return true;
}

