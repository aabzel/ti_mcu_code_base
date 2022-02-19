#include "test_params.h"

#include <stdbool.h>
#include <stdint.h>

#include "param_ids.h"
#include "param_types.h"
#include "unit_test_check.h"

bool test_params(void) {
    uint8_t value[5]={0x0f,0xc0,0xca,0x89,0x36};
    char str[40];
    EXPECT_TRUE( raw_val_2str(value, 1, UINT8 , str, sizeof(str)));
    EXPECT_STREQ("15",str);
    EXPECT_TRUE( raw_val_2str(value, 1, INT8 , str, sizeof(str)));
    EXPECT_STREQ("15",str);

    EXPECT_TRUE( raw_val_2str(&value[1], 4, UINT32 , str, sizeof(str)));
    EXPECT_STREQ("915000000",str);
    return true;
}


bool test_param_types(void){
    EXPECT_EQ(2,sizeof(LinkInfoId_t));

    return true;
}
 
