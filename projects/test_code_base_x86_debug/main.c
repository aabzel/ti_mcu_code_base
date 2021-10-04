#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#include "test_rtcm3_proto.h"
#include "unit_test_check.h"

static bool unit_test_sw_run(void){
	bool res = false;
	EXPECT_TRUE( test_rtcm3_proto());
	return true;
}


int main(int argc, char* argv[]) {
	int ret=0;
    bool res = false;
    printf("test code base!\n");
	EXPECT_TRUE( unit_test_sw_run());

    printf("End program.\n");
    return ret;
}
