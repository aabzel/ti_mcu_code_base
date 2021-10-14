#include "test_tcan4550.h"

#include <string.h>
#include <gpio.h>

#include "tcan4550_diag.h"
#include "tcan4550_drv.h"
#include "unit_test_check.h"

bool test_can_detect(void) {
    EXPECT_TRUE( is_tcan4550_connected());
    return true;
}

static bool test_can_write_one(uint32_t write_val) {
    uint32_t read_reg=0;
    EXPECT_TRUE( tcan4550_write_reg(ADDR_READ_WRITE_TEST, write_val)) ;
    EXPECT_TRUE( tcan4550_read_reg(ADDR_READ_WRITE_TEST, &read_reg)) ;
    EXPECT_EQ(write_val, read_reg);
    return true;
}

bool test_can_write(void) {
    EXPECT_TRUE(test_can_write_one(0x55555555));
    EXPECT_TRUE(test_can_write_one(0xAAAAAAAA));
    return true;
}

bool test_can_read_word(void) {
    uint32_t read_dev_id0_4 = 0;
    EXPECT_TRUE( tcan4550_read(ADDR_DEVICE_ID_0, 1, (uint8_t *)&read_dev_id0_4, sizeof(read_dev_id0_4)));
    EXPECT_EQ(0x5443414E, read_dev_id0_4);
    return true;
}

bool test_can_types(void) {
    EXPECT_EQ(4, sizeof(HeaderCom_t));
    EXPECT_EQ(4, sizeof(tCanRegRev_t));
    EXPECT_EQ(4, sizeof(tCanRegStatus_t));

    return true;
}
