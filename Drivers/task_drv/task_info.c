#include "task_info.h"

#include <inttypes.h>

#include "base_cmd.h"
#include "clocks.h"
#include "data_utils.h"
#include "diag_page_nums.h"
#include "diag_report.h"
#include "flash_drv.h"
#include "sys.h"
#include "sys_tick.h"
#include "table_utils.h"
#include "writer_generic.h"

uint64_t total_time0_us = 0;
uint64_t total_time_ms0 = 0;

#ifdef HAS_DEBUG
uint32_t iteration_cnt = 0;
uint64_t loop_duration_us = 0;
uint64_t loop_duration_min_us = UINT64_MAX;
uint64_t loop_duration_max_us = 0;
#endif /*HAS_DEBUG*/

#ifdef TASKS
task_data_t task_data[] = {
#define TASK(task_name) {.name = #task_name},
    TASKS
#undef TASK
};
#endif /* TASKS */


uint16_t task_cnt(void){
    uint16_t cnt = ARRAY_SIZE(task_data);
    return cnt;
}

uint64_t calc_total_run_time(void) {
    uint64_t tot_run_time = 0;
    int32_t id = 0;
    for(id = 0; id < TASK_ID_COUNT; id++) {
        tot_run_time += task_data[id].run_time_total;
    }
    if(0 == tot_run_time) {
        task_init();
        tot_run_time = UINT64_MAX;
    }
    return tot_run_time;
}

bool task_frame(task_data_t* taskItem, bool (*task_func)(void)) {
    bool res = false;
    if(taskItem){
        taskItem->start_count++;
        uint64_t stop = 0, delta = 0, period = 0;
        uint64_t start = get_time_us();
        if(taskItem->start_time_prev < start) {
            period = start - taskItem->start_time_prev;
            res = true;
        } else {
            period = 0; /*(0x1000000U + start) - TASK_ITEM.start_time_prev; */
            res = false;
        }
        taskItem->start_time_prev = start;
        if(taskItem->init) {
            taskItem->start_period_max = rx_max64u(taskItem->start_period_max, period);
            taskItem->start_period_min = rx_min64u(taskItem->start_period_min, period);
        }
        taskItem->init = true;
        if(true==is_flash_addr((uint32_t)task_func)){
            res = task_func();
        }else{
            res = false;
        }
        stop = get_time_us();
        if(start < stop) {
            delta = stop - start;
            res = true;
        } else {
            delta = 0; /*(0x1000000U + stop) - start;*/
            res = false;
        }
        taskItem->run_time_total += delta;
        taskItem->run_time_min = rx_min64u(taskItem->run_time_min, delta);
        taskItem->run_time_max = rx_max64u(taskItem->run_time_max, delta);
    }
    return res;
}

static bool _measure_task_interval(task_data_t* taskItem, uint64_t interval_us, bool (*task_func)(void),
                                   uint64_t loop_start_time_us) {
    bool res = false;
    if(taskItem->start_time_next < loop_start_time_us) {
        taskItem->start_time_next = loop_start_time_us + interval_us;
        res = task_frame(taskItem, task_func);
    }
    return res;
}

bool measure_task_interval(uint16_t task_id, uint64_t interval_us, bool (*task_func)(void),
                           uint64_t loop_start_time_us) {
    bool res = false;
    if(task_data[task_id].on){
        res = _measure_task_interval(&task_data[task_id], interval_us, task_func, loop_start_time_us);
    }
    return res;
}

bool task_init(void) {

#ifdef HAS_DEBUG
    iteration_cnt = 0;
    loop_duration_us = 0;
    loop_duration_min_us = UINT64_MAX;
    loop_duration_max_us = 0;
#endif /*HAS_DEBUG*/
    int32_t id = 0;
    for(id = 0; id < TASK_ID_COUNT; id++) {
        task_data[id].start_count = 0;
        task_data[id].run_time_total = 0;
        task_data[id].run_time_min = UINT64_MAX;
        task_data[id].run_time_max = 0;
        task_data[id].start_period_min = UINT64_MAX;
        task_data[id].start_period_max = 0;
        task_data[id].on=true;
    }
    total_time0_us = get_time_us();
    total_time_ms0 = get_time_ms64();

    return true;
}
