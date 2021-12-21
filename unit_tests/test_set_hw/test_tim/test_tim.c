#include "test_tim.h"

#include "clocks.h"
#include "tim_drv.h"
//#include "sys_config.h"
#include "none_blocking_pause.h"
#include "unit_test_check.h"

bool test_tim_us(void) {
    uint32_t i = 0;
    uint64_t up_time_us_cur = 0;
    uint64_t up_time_us_prev = 0;
    wait_in_loop_ms(20) ;
    up_time_us_prev = tim_get_us();
    pause_1us();
    for(i = 0; i < 10000; i++) {
        up_time_us_cur = tim_get_us();
        EXPECT_GR(up_time_us_prev, up_time_us_cur, i);
        pause_1us();
        pause_1us();
        up_time_us_prev = up_time_us_cur;
    }
    return true;
}

bool test_tim_ms(void) {
    uint32_t i = 0;
    uint32_t up_time_ms_cur = 0;
    uint32_t up_time_ms_prev = 0;
    up_time_ms_prev = tim_get_ms();
    pause_1ms();
    for(i = 0; i < 500; i++) {
        up_time_ms_cur = tim_get_ms();
        EXPECT_GR(up_time_ms_prev, up_time_ms_cur, i);
        pause_1ms();
        pause_1ms();
        up_time_ms_prev = up_time_ms_cur;
    }
    return true;
}
