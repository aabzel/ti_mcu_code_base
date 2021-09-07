#ifndef SRC_TASK_INFO_H
#define SRC_TASK_INFO_H

#include <stdint.h>

#include "oprintf.h"
#include "sys.h"
#include "data_utils.h"
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
extern uint32_t iteration_cnt ;
extern task_data_t task_data[];

typedef enum {
#define TASK(id) TASK_ID_##id,
    TASKS
#undef TASK
        TASK_ID_COUNT
} task_id_t;
#endif /* TASKS */

#define TASK_CHECK_UNIQUE_ID(id)                                                                                       \
    typedef uint32_t TASK_CHECK_UNIQUE_ID_##id;                                                                        \
    TASK_CHECK_UNIQUE_ID_##id task_check_unique_id_##id = 0U;                                                          \
    (void)task_check_unique_id_##id;

#define MAIN_LOOP_START                                                                                                \
        iteration_cnt++;                                                                                               \
        uint64_t loop_start_time_us = get_time_us();                                                                   \
        (void)loop_start_time_us;



#define _TASK_START(TASK_ITEM)                                                                                         \
    {                                                                                                                  \
        TASK_ITEM.start_count++;                                                                                       \
        uint64_t stop = 0, delta = 0, period = 0, start = get_time_us();                                               \
        if(TASK_ITEM.start_time_prev < start) {                                                                        \
            period = start - TASK_ITEM.start_time_prev;                                                                \
        } else {                                                                                                       \
            period = 0; /*(0x1000000U + start) - TASK_ITEM.start_time_prev; */                                         \
        }                                                                                                              \
        TASK_ITEM.start_time_prev = start;                                                                             \
        if(TASK_ITEM.init) {                                                                                           \
            TASK_ITEM.start_period_max = rx_max64u(TASK_ITEM.start_period_max, period);                               \
            TASK_ITEM.start_period_min = rx_min64u(TASK_ITEM.start_period_min, period);                               \
        }                                                                                                              \
        TASK_ITEM.init = true;

#define _TASK_STOP(TASK_ITEM)                                                                                          \
        stop = get_time_us();                                                                                          \
        if(start < stop) {                                                                                             \
            delta = stop - start;                                                                                      \
        } else {                                                                                                       \
            delta = 0;/*(0x1000000U + stop) - start;*/                                                                 \
        }                                                                                                              \
        TASK_ITEM.run_time_total += delta;                                                                             \
        TASK_ITEM.run_time_min = rx_min64u(TASK_ITEM.run_time_min, delta);                                             \
        TASK_ITEM.run_time_max = rx_max64u(TASK_ITEM.run_time_max, delta);                                             \
    }

#define _MEASURE_TASK_INTERVAL(TASK_ITEM, interval_us, task_func)                                                      \
    do {                                                                                                               \
        if(TASK_ITEM.start_time_next < loop_start_time_us) {                                                           \
            TASK_ITEM.start_time_next = loop_start_time_us + interval_us;                                              \
            _TASK_START(TASK_ITEM)                                                                                     \
            task_func();                                                                                               \
            _TASK_STOP(TASK_ITEM)                                                                                      \
        }                                                                                                              \
    } while(0);

#define _MEASURE_TASK(TASK_ITEM, task_func)                                                                            \
    do{                                                                                                                \
        _TASK_START(TASK_ITEM)                                                                                         \
        task_func();                                                                                                   \
        _TASK_STOP(TASK_ITEM)                                                                                          \
    }while(0);


#define _MEASURE_TASK_INTERVAL_OLD(TASK_ITEM, interval_us, task_func)                                                  \
    {                                                                                                                  \
        if(loop_start_time_us > TASK_ITEM.start_time_next) {                                                           \
            TASK_ITEM.start_time_next += US_TO_COUNTER(interval_us);                                                   \
            _TASK_START(TASK_ITEM)                                                                                     \
            task_func();                                                                                               \
            _TASK_STOP(TASK_ITEM)                                                                                      \
        }                                                                                                              \
    }

/* Default core task API */
#define MEASURE_TASK(id, task_func)                                                                                    \
    TASK_CHECK_UNIQUE_ID(TASK_ID_##id)                                                                                 \
    _MEASURE_TASK(task_data[TASK_ID_##id], task_func)

#define MEASURE_TASK_INTERVAL(id, interval_us, task_func)                                                              \
    TASK_CHECK_UNIQUE_ID(TASK_ID_##id)                                                                                 \
    _MEASURE_TASK_INTERVAL(task_data[TASK_ID_##id], interval_us, task_func)

#define MEASURE_TASK_INTERVAL_OLD(id, interval_us, task_func)                                                          \
    TASK_CHECK_UNIQUE_ID(TASK_ID_##id)                                                                                 \
    _MEASURE_TASK_INTERVAL_OLD(task_data[TASK_ID_##id], interval_us, task_func)

#ifdef TASKS
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
