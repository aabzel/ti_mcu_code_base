#include "task_commands.h"

#include <inttypes.h>

#include "base_cmd.h"
#include "clocks.h"
#include "convert.h"
#include "data_utils.h"
#include "log.h"
#include "sys.h"
#include "sys_tick.h"
#include "task_info.h"
#include "table_utils.h"
#include "writer_generic.h"
#include "writer_config.h"

bool diag_page_tasks(ostream_t* stream) {
    stream = &(curWriterPtr->s);
    uint64_t total_time_us = 0;
    uint64_t total_time_ms = 0;
    uint64_t total_run_time_us = 0;
    int32_t id = 0;
    total_time_us = get_time_us() - total_time0_us;
    total_time_ms = get_time_ms64() - total_time_ms0;

    if(0 == total_time_ms) {
        return false;
    }

    total_run_time_us = calc_total_run_time();
    uint16_t t_cnt=task_cnt();
#ifdef HAS_DEBUG
    oprintf(stream, "task cnt %u" CRLF, t_cnt);
    oprintf(stream, "loop duration %u us" CRLF, loop_duration_us);
    oprintf(stream, "max loop duration %u us" CRLF, loop_duration_max_us);
    oprintf(stream, "min loop duration %u us" CRLF, loop_duration_min_us);
    oprintf(stream, "iteration cnt %u" CRLF, iteration_cnt);
#endif /*HAS_DEBUG*/
    oprintf(stream, "total run time  %u us" CRLF, total_run_time_us);
    oprintf(stream, "up_time  %u ms" CRLF, g_up_time_ms);
    oprintf(stream, "total_time %llu us" CRLF, total_time_us);
    oprintf(stream, "total_time %llu ms" CRLF, total_time_ms);
    static const table_col_t cols[] = {{4, "id"}, {11, "Task name"}, {8, "Calls/s"},   {7, "CPU[%]"},
                                       {11, "Tavg[ms]"},  {11, "Tmin[ms]"}, {11, "Tmax[ms]"},
                                       {11, "Rmin[us]"},  {11, "Rmax[us]"}, {11, "RTtot[us]"},{6, "stat"}};
    float cpu_use = 0.0f;
    table_header(stream, cols, ARRAY_SIZE(cols));
    for(id = 0; id < t_cnt  ; id++) {
        if(0 != task_data[id].start_count) {
            cpu_use = (((float)task_data[id].run_time_total) * 100.0f) / ((float)total_run_time_us);
            oprintf(stream, TABLE_LEFT " %2u " TABLE_SEPARATOR, id);
            oprintf(stream,
                     "%10s " TABLE_SEPARATOR
                     "%8" PRIu32 TABLE_SEPARATOR
                     "%7.3f" TABLE_SEPARATOR
                               "%11" PRIu32 TABLE_SEPARATOR,
                               task_data[id].name,
                               (uint32_t)((task_data[id].start_count * 1000) / total_time_ms),
                               cpu_use,
                    (uint32_t)((total_time_ms * 1000) / task_data[id].start_count)/1000);
            oprintf(stream, " %9" PRIu64 " " TABLE_SEPARATOR, task_data[id].start_period_min/1000);
            oprintf(stream, " %9" PRIu64 " " TABLE_SEPARATOR, task_data[id].start_period_max/1000);
            oprintf(stream, " %9" PRIu64 " " TABLE_SEPARATOR, task_data[id].run_time_min);
            oprintf(stream, " %9" PRIu64 " " TABLE_SEPARATOR, task_data[id].run_time_max);
            oprintf(stream, " %9" PRIu64 " " TABLE_SEPARATOR, task_data[id].run_time_total);
            oprintf(stream, "  %2u  " TABLE_SEPARATOR, task_data[id].on);
            oprintf(stream, CRLF);
        }
    }
    table_row_bottom(stream, cols, ARRAY_SIZE(cols));

    oprintf(stream, "Rmax - Maximum task continuous run time," CRLF "Tavg - Average task execution period," CRLF
                    "Tmax - Maximum task execution period" CRLF);

    return true;
}

bool cmd_task_ctrl(int32_t argc, char* argv[]){
    bool res = false;
    bool task_state = true;
    uint16_t task_id = 0;
    if(1<=argc){
        res = try_str2uint16(argv[0], &task_id);
        if(false == res) {
            LOG_ERROR(SYS, "Unable to extract task_id %s", argv[0]);
        }
    }
    if(2<=argc){
        res = try_str2bool(argv[1], &task_state);
        if(false == res) {
            LOG_ERROR(SYS, "Unable to extract task_state %s", argv[1]);
        }
    }
    if(res){
        uint16_t cnt=task_cnt();
        if(task_id<cnt){
           task_data[task_id].on = task_state;
           LOG_INFO(SYS, "task %s %s", task_data[task_id].name, (true==task_state)?"on":"off");
        }
    }else{
        LOG_ERROR(SYS, "Unable control task");
    }
    return res;
}

bool cmd_task_report(int32_t argc, char* argv[]) {
    (void)(argc);
    (void)(argv);
    bool res = false;
    res = diag_page_tasks(DBG_UART_STREAM);
    return res;
}

bool cmd_task_clear(int32_t argc, char* argv[]) {
    (void)(argc);
    (void)(argv);
    bool res = false;
    res = task_init();
    return res;
}
