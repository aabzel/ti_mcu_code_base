#include "test_tcan4550.h"

#include <string.h>
#include <gpio.h>

#include "tcan4550_drv.h"
#include "unit_test_check.h"

bool test_can_detect(void) {
    EXPECT_TRUE( is_tcan4550_connected());
    return true;
}

bool test_can_write(void) {
    return true;
}

bool test_can_read_word(void) {
    uint32_t read_dev_id0_4 = 0;
    EXPECT_TRUE( tcan4550_read(DEVICE_ID_0, 1, (uint8_t *)&read_dev_id0_4, sizeof(read_dev_id0_4)));
    EXPECT_EQ(0x5443414E, read_dev_id0_4);
    return true;
}
