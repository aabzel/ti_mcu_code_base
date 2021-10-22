#include "test_gpio.h"

#include <string.h>
#include <gpio.h>

#include "gpio_drv.h"
#include "sys_config.h"
#include "clocks.h"
#include "log.h"
#include "unit_test_check.h"

static bool test_gpio_pin_lev(uint8_t dio_number,uint8_t des_logic_level) {
    uint8_t cur_logic_level;
    EXPECT_TRUE( gpio_set_state(dio_number, des_logic_level));
    delay_ms(200);
    EXPECT_TRUE(  gpio_get_state(dio_number, &cur_logic_level) );
    EXPECT_EQ(des_logic_level,cur_logic_level);
    return true;
}

static bool test_gpio_pin(uint8_t dio_number) {
    uint8_t origin_logic_level = 0;
    EXPECT_TRUE(  gpio_get_state(dio_number, &origin_logic_level) );

    EXPECT_TRUE(test_gpio_pin_lev(dio_number,0));
    EXPECT_TRUE( test_gpio_pin_lev(dio_number,1));

    EXPECT_TRUE( gpio_set_state(dio_number, origin_logic_level));

    LOG_INFO(TEST,"wire  DIO_%2u %s Fine!", dio_number, gpio_get_name(dio_number));
    return true;
}

bool test_gpio_write(void) {
    EXPECT_TRUE(test_gpio_pin(DIO_CAN_SS));
    EXPECT_TRUE(test_gpio_pin(DIO_CAN_RST));
    EXPECT_TRUE(test_gpio_pin(DIO_GNSS_RST_N));
    EXPECT_TRUE(test_gpio_pin(DIO_SX1262_RST));
    EXPECT_TRUE(test_gpio_pin(DIO_SX1262_SS));
    EXPECT_TRUE(test_gpio_pin(DIO_LED_RED ));
    EXPECT_TRUE(test_gpio_pin(DIO_LED_GREEN ));

#ifdef HAS_IO_BANG
    EXPECT_TRUE(test_gpio_pin(DIO_SCLK));
    EXPECT_TRUE(test_gpio_pin(DIO_MOSI));
    EXPECT_TRUE(test_gpio_pin(DIO_BATT_SCL));
    EXPECT_TRUE(test_gpio_pin(DIO_BATT_SDA));
    EXPECT_TRUE(test_gpio_pin(DIO_GNSS_INT));
    EXPECT_TRUE(test_gpio_pin(DIO_GNSS_RXD ));
    EXPECT_TRUE(test_gpio_pin(DIO_GNSS_SAFEBOOT_N));
    EXPECT_TRUE(test_gpio_pin(DIO_PS_RS232 ));
    EXPECT_TRUE(test_gpio_pin(DIO_PWR_MUX_CTRL ));
#endif
    return true;
}
#if 0
bool test_gpio_read(void) {

    return true;
}
#endif
