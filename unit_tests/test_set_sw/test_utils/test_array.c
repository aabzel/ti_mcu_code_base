#include "test_array.h"

#include "array.h"
#include "unit_test_check.h"

const uint8_t array1[]={0,0,0,2,3,4};

bool test_array_con_pat(void){
    uint32_t max_cont_patt=0;
    EXPECT_TRUE( array_max_cont((uint8_t*) array1, sizeof(array1), 0, &max_cont_patt));
    EXPECT_EQ( 3, max_cont_patt);
	return true;
}
