#include "test_gpio.h"

#include <string.h>
#include <gpio.h>

#include "gpio_drv.h"
#include "sys_config.h"
#include "clocks.h"
#include "unit_test_check.h"

static bool test_gpio_pin_lev(uint8_t dio_number,uint8_t des_logic_level) {
    uint8_t cur_logic_level;
    EXPECT_TRUE( gpio_set_state(dio_number, des_logic_level));
    delay_ms(2);
    EXPECT_TRUE(  gpio_get_state(dio_number, &cur_logic_level) );
    EXPECT_EQ(des_logic_level,cur_logic_level);
    return true;
}

static bool test_gpio_pin(uint8_t dio_number) {
    uint8_t origin_logic_level=0;
    EXPECT_TRUE(  gpio_get_state(dio_number, &origin_logic_level) );

    EXPECT_TRUE(test_gpio_pin_lev(dio_number,0));
    EXPECT_TRUE( test_gpio_pin_lev(dio_number,1));

    EXPECT_TRUE( gpio_set_state(dio_number, origin_logic_level));
    return true;
}

bool test_gpio_write(void) {
    EXPECT_TRUE(test_gpio_pin(DIO_CAN_SS));
    EXPECT_TRUE(test_gpio_pin(DIO_CAN_RST));
    EXPECT_TRUE(test_gpio_pin(DIO_GNSS_RST_N));
    EXPECT_TRUE(test_gpio_pin(DIO_SX1262_RST));
    EXPECT_TRUE(test_gpio_pin(DIO_SX1262_SS));
    return true;
}
#if 0
bool test_gpio_read(void) {

    return true;
}
#endif
