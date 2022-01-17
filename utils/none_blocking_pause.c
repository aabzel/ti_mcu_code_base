#include "none_blocking_pause.h"

#include "clocks.h"
#include "common_functions.h"
#ifdef NORTOS
#include "task_info.h"
#endif
#include "timer_utils.h"

/* 49 days max*/
/*none blocking wait for self test and polling*/
bool wait_in_loop_ms(uint32_t wait_pause_ms) {
    uint32_t start_ms = 0U;
    uint32_t curr_ms = 0U;
    bool res = false;
#ifdef HAS_FREE_RTOS
    // vTaskDelay(wait_pause_ms / portTICK_RATE_MS);
#endif
    start_ms = get_time_ms32();
    bool loop = true;
#ifdef HAS_SUPER_LOOP
    uint64_t loop_start_time_us = 0;
#endif
    while(loop) {
#ifdef HAS_SUPER_LOOP
        loop_start_time_us = get_time_us();
        super_loop(loop_start_time_us);
#endif
        curr_ms = get_time_ms32();
        if(wait_pause_ms < (curr_ms - start_ms)) {
            res = true;
            loop = false;
            break;
        }
    }
    return res;
}

bool wait_ms(int32_t wait_pause_ms) {
    uint32_t start_ms = 0U;
    uint32_t curr_ms = 0U;
    uint32_t cnt = 0;
    bool res = false;
    start_ms = get_time_ms32();
    bool loop = true;
    int32_t diff_ms = 0;
    while(loop) {
        curr_ms = get_time_ms32();
        diff_ms = curr_ms - start_ms;
        if(wait_pause_ms < diff_ms) {
            res = true;
            loop = false;
            break;
        }
        cnt++;
        if((2500 * wait_pause_ms) < cnt) {
            res = false;
            loop = false;
            break;
        }
    }
    return res;
}

/*none blocking wait for self test and poling*/
bool wait_us(uint64_t wait_pause_us) {
    uint64_t start_us = 0U;
    uint64_t curr_us = 0U;
    uint64_t diff_us = 0U;
    bool res = false;
    start_us = get_time_us();
    bool loop = true;
    while(loop) {
        curr_us = get_time_us();
        diff_us = curr_us - start_us;
        if(wait_pause_us < diff_us) {
            res = true;
            loop = false;
            break;
        }
    }
    return res;
}
