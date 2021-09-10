#ifndef SRC_TASK_INFO_H
#define SRC_TASK_INFO_H

#include <stdint.h>

#include "data_utils.h"
#include "oprintf.h"
#include "sys.h"
#include "task_config.h"

/*some cooperative RTOS implementation*/

typedef struct task_data_tag {
    bool init;
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
extern uint32_t iteration_cnt;
extern uint64_t loop_duration_us;
extern task_data_t task_data[];

typedef enum {
#define TASK(id) TASK_ID_##id,
    TASKS
#undef TASK
        TASK_ID_COUNT
} task_id_t;
#endif /* TASKS */

#ifdef TASKS
bool measure_task_interval(uint16_t task_id, uint64_t interval_us, bool (*task_func)(void),
                           uint64_t loop_start_time_us);
bool diag_page_tasks(ostream_t* stream);
bool cmd_task_report(int32_t argc, char* argv[]);
bool cmd_task_clear(int32_t argc, char* argv[]);

#define TASK_COMMANDS                                                                                                  \
    SHELL_CMD("task_report", "ti", cmd_task_report, "Task execution time report"),                                     \
        SHELL_CMD("task_clear", "tic", cmd_task_clear, "Clear task execution info"),

#else
#define TASK_COMMANDS
#endif /*TASKS*/

#endif /* SRC_TASK_INFO_H */
