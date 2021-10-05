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

    uint32_t cnt = get_test_list_cnt();
    printf("number of tests: %u\n", cnt);

    uint32_t index = 0;
    for(index = 0; index < cnt; index++) {
        unit_test_run(index);
    };

    printf("\nEnd program.\n");
    return ret;
}
