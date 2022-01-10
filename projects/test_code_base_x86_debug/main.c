#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#include "log.h"
#include "test_list.h"
#include "test_rtcm3_proto.h"
#include "unit_test_check.h"
#include "unit_test_run.h"
#include "win_utils.h"

int main(int argc, char* argv[]) {
    int ret = 0;
    bool res = false;
    win_color_enable();
    set_log_level(SYS, LOG_LEVEL_DEBUG);
    LOG_INFO(SYS,"Test Code Base:");
    LOG_INFO(SYS,"STDC_VER : %u", __STDC_VERSION__);

    uint32_t cnt = get_test_list_cnt();
    LOG_INFO(SYS,"number of tests: %u", cnt);
    bool rest_res = false;
    uint32_t failed_cnt = 0, total = 0;
    uint32_t passed_cnt = 0;
    uint32_t index = 0;
    unit_test_info_t* ui = NULL;
    for(index = 0; index < cnt; index++) {
        rest_res = unit_test_run(index);
        if(false == rest_res) {
            failed_cnt++;
            ui = get_unit_test(index);
            LOG_ERROR(SYS,"Tests failed index: %u [%s]", index, ui->name);
        } else {
            passed_cnt++;
        }
        total++;
    };
    if(0 < failed_cnt) {
        LOG_ERROR(SYS,"Tests Failure: %u/%u", failed_cnt, total);
    } else {
        LOG_INFO(SYS,"Tests Passed: %u", total);
    }
    LOG_INFO(SYS,"End program.");
    return ret;
}
