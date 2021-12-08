#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#include "test_list.h"
#include "test_rtcm3_proto.h"
#include "unit_test_check.h"
#include "unit_test_run.h"

int main(int argc, char* argv[]) {
    int ret = 0;
    bool res = false;
    printf("test code base!\n");
    printf("STDC_VER : %u \n", __STDC_VERSION__);

    uint32_t cnt = get_test_list_cnt();
    printf("number of tests: %u\n", cnt);
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
            printf("\n[e]    nTests failed index: %u [%s]\n", index, ui->name);
        } else {
            passed_cnt++;
        }
        total++;
    };
    if(0 < failed_cnt) {
        printf("\n\n\n[e] Tests Failure: %u/%u\n", failed_cnt, total);
    } else {
        printf("\n\n\n[*] Tests Passed: %u \n", total);
    }
    printf("\nEnd program.\n");
    return ret;
}
