#ifndef SRC_TASK_INFO_H
#define SRC_TASK_INFO_H

#include <stdint.h>

#include "data_utils.h"
#include "oprintf.h"
#include "sys.h"
#include "task_config.h"

/*some cooperative RTOS implementation*/
extern uint64_t total_time0_us;
extern uint64_t total_time_ms0;

typedef struct task_data_tag {
    bool init;
    bool on;
    const char* const name;
    uint64_t run_time_total; /*us*/
    uint64_t start_count;
    uint64_t start_time_prev;  /*us*/
    uint64_t start_time_next;  /*us*/
    uint64_t start_period_min; /*us*/
    uint64_t start_period_max; /*us*/
    uint64_t run_time_max;     /*us*/
    uint64_t run_time_min;     /*us*/
} task_data_t;

#ifdef TASKS

#ifdef HAS_DEBUG
extern uint32_t iteration_cnt;
extern uint64_t loop_duration_us;
extern uint64_t loop_duration_min_us;
extern uint64_t loop_duration_max_us;
#endif /*HAS_DEBUG*/

extern task_data_t task_data[];

typedef enum {
#define TASK(id) TASK_ID_##id,
    TASKS
#undef TASK
        TASK_ID_COUNT
} task_id_t;
#endif /* TASKS */

#ifdef TASKS
uint16_t task_cnt(void);
uint64_t calc_total_run_time(void) ;
bool task_init(void);
bool measure_task_interval(uint16_t task_id, uint64_t interval_us, bool (*task_func)(void),
                           uint64_t loop_start_time_us);
bool diag_page_tasks(ostream_t* stream);
#endif /*TASKS*/

#endif /* SRC_TASK_INFO_H */
