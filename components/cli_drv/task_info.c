#include "task_info.h"

#include <inttypes.h>

#include "clocks.h"
#include "data_utils.h"
#include "diag_page_nums.h"
#include "diag_report.h"
#include "shell.h"
#include "sys.h"
#include "table_utils.h"
#include "writer_generic.h"

static uint64_t total_time0;
static uint64_t total_time_ms0;

#ifdef TASKS
task_data_t task_data[] = {
#define TASK(task_name) {.name = #task_name},
    TASKS
#undef TASK
};
#endif /* TASKS */

bool diag_page_tasks(ostream_t* stream) {
    stream = &dbg_o.s;
    static const table_col_t cols[] = {
        {22, "Task name"}, {8, "Calls/s"}, {6, "CPU[%]"}, {12, "Rmax[us]"}, {12, "Tavg[us]"}, {12, "Tmax[us]"},
    };

    uint64_t total_time;
    uint64_t total_time_ms;
    uint8_t core=0;
    int32_t id;

    total_time = getRunTimeCounterValue64() - total_time0;
    total_time_ms = get_time_ms64() - total_time_ms0;

    const task_data_t* data[] = {
#ifdef TASKS
        task_data,
#endif
        NULL};

    const int32_t count[] = {
#ifdef TASKS
        TASK_ID_COUNT,
#endif
        0};

    if (total_time_ms == 0U) {
        return false;
    }

    for (core = 0; core < ARRAY_SIZE(data) - 1U; core++) {
        table_header(stream, cols, ARRAY_SIZE(cols));
        for (id = 0; id < count[core]; id++) {
            if (data[core][id].start_count != 0) {
                uint16_t cpu_e3 = data[core][id].run_time_total * 1000 / total_time;
                oprintf(stream,
                        TABLE_LEFT "%21s " TABLE_SEPARATOR "%8" PRIu32 TABLE_SEPARATOR "%4u.%u" TABLE_SEPARATOR
                                   "%12" PRIu32 TABLE_SEPARATOR "%12" PRIu32 TABLE_SEPARATOR
                                   "%12" PRIu32 TABLE_RIGHT CRLF,
                        data[core][id].name, (uint32_t)((data[core][id].start_count * 1000) / total_time_ms),
                        cpu_e3 / 10, cpu_e3 % 10, (uint32_t)(COUNTER_TO_US(data[core][id].run_time_max)),
                        (uint32_t)((total_time_ms * 1000) / data[core][id].start_count),
                        (uint32_t)(COUNTER_TO_US(data[core][id].start_period_max)));
            }
        }
        table_row_bottom(stream, cols, ARRAY_SIZE(cols));
        oprintf(stream, CRLF);
    }

    oprintf(stream, "Rmax - Maximum task continuous run time," CRLF "Tavg - Average task execution period," CRLF
                    "Tmax - Maximum task execution period" CRLF);

    return true;
}

bool cmd_task_clear(int32_t argc, char* argv[]) {
    (void)(argc);
    (void)(argv);

#ifdef TASKS
    int32_t id = 0;
    for (id = 0; id < TASK_ID_COUNT; id++) {
        task_data[id].start_count = 0;
        task_data[id].run_time_total = 0;
        task_data[id].run_time_max = 0;
        task_data[id].start_period_max = 0;
    }
#endif

    total_time0 = getRunTimeCounterValue64();
    total_time_ms0 = get_time_ms64();

    return dump_cmd_result(true);
}

bool cmd_task_report(int32_t argc, char* argv[]) {
    (void)(argc);
    (void)(argv);
    return show_diag_report(DIAG_PAGE_TASKS);
}
