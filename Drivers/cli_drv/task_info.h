#ifndef SRC_TASK_INFO_H
#define SRC_TASK_INFO_H

#ifdef HAS_CLI

#include <stdint.h>

#include "oprintf.h"
#include "sys.h"
#include "task_config.h"

typedef struct task_data_tag {
    bool init;
    const char* const name;
    uint64_t run_time_total;
    uint64_t start_count;
    uint64_t start_time_prev;
    uint64_t start_time_next;
    uint64_t start_period_max;
    uint64_t run_time_max;
} task_data_t;

#ifdef TASKS
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
    uint64_t loop_start_time = getRunTimeCounterValue64();                                                             \
    (void)loop_start_time;

#define _TASK_START(data)                                                                                              \
    {                                                                                                                  \
        data.start_count++;                                                                                            \
        uint32_t stop, delta, period, start = getRunTimeCounterValue32();                                              \
        if (start > data.start_time_prev) {                                                                            \
            period = start - data.start_time_prev;                                                                     \
        } else {                                                                                                       \
            period = (0x100000000U + start) - data.start_time_prev;                                                    \
        }                                                                                                              \
        data.start_time_prev = start;                                                                                  \
        if ((data.start_period_max < period) && data.init) {                                                           \
            data.start_period_max = period;                                                                            \
        }                                                                                                              \
        data.init = true;

#define _TASK_STOP(data)                                                                                               \
    stop = getRunTimeCounterValue32();                                                                                 \
    if (stop > start) {                                                                                                \
        delta = stop - start;                                                                                          \
    } else {                                                                                                           \
        delta = (0x100000000U + stop) - start;                                                                         \
    }                                                                                                                  \
    data.run_time_total += delta;                                                                                      \
    if (data.run_time_max < delta) {                                                                                   \
        data.run_time_max = delta;                                                                                     \
    }                                                                                                                  \
    }

#define _MEASURE_TASK(data, task_func)                                                                                 \
    {                                                                                                                  \
        _TASK_START(data)                                                                                              \
        task_func();                                                                                                   \
        _TASK_STOP(data)                                                                                               \
    }

#define _MEASURE_TASK_INTERVAL(data, interval_us, task_func)                                                           \
    {                                                                                                                  \
        if (loop_start_time > data.start_time_next) {                                                                  \
            data.start_time_next = loop_start_time + US_TO_COUNTER(interval_us);                                       \
            _TASK_START(data)                                                                                          \
            task_func();                                                                                               \
            _TASK_STOP(data)                                                                                           \
        }                                                                                                              \
    }

#define _MEASURE_TASK_INTERVAL_OLD(data, interval_us, task_func)                                                       \
    {                                                                                                                  \
        if (loop_start_time > data.start_time_next) {                                                                  \
            data.start_time_next += US_TO_COUNTER(interval_us);                                                        \
            _TASK_START(data)                                                                                          \
            task_func();                                                                                               \
            _TASK_STOP(data)                                                                                           \
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

#endif /*HAS_CLI*/

#endif /* SRC_TASK_INFO_H */
