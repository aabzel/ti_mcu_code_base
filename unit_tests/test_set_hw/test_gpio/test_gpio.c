#include "test_gpio.h"

#include <string.h>

#include "clocks.h"
#include "gpio_drv.h"
#include "log.h"
#include "sys_config.h"
#include "unit_test_check.h"

static bool test_gpio_pin_lev(uint8_t dio_number, uint8_t des_logic_level) {
    uint8_t cur_logic_level = 0xFF;
    EXPECT_TRUE(gpio_set_state(dio_number, des_logic_level));
    delay_ms(300);
    EXPECT_TRUE(gpio_get_state(dio_number, &cur_logic_level));
    EXPECT_EQ(des_logic_level, cur_logic_level);
    LOG_INFO(TEST, "set DIO_%2u %s to %u OK!", dio_number, gpio_get_name(dio_number), des_logic_level);
    return true;
}

static bool test_gpio_pin(uint8_t dio_number) {
    uint8_t origin_logic_level = 0;
#ifndef ESP32
    EXPECT_EQ(GPIO_DIR_INOUT, gpio_get_dir(dio_number));
#endif
    EXPECT_TRUE(gpio_get_state(dio_number, &origin_logic_level));

    EXPECT_TRUE(test_gpio_pin_lev(dio_number, 0));
    EXPECT_TRUE(test_gpio_pin_lev(dio_number, 1));

    EXPECT_TRUE(gpio_set_state(dio_number, origin_logic_level));

    LOG_INFO(TEST, "wire  DIO_%2u %s Fine!", dio_number, gpio_get_name(dio_number));
    return true;
}

bool test_gpio_write(void) {
#ifdef DIO_CAN_SS
    EXPECT_TRUE(test_gpio_pin(DIO_CAN_SS));
#endif
#ifdef DIO_CAN_RST
    EXPECT_TRUE(test_gpio_pin(DIO_CAN_RST));
#endif
#ifdef DIO_GNSS_RST_N
    EXPECT_TRUE(test_gpio_pin(DIO_GNSS_RST_N));
#endif
    EXPECT_TRUE(test_gpio_pin(DIO_SX1262_RST));
    EXPECT_TRUE(test_gpio_pin(DIO_SX1262_SS));
    EXPECT_TRUE(test_gpio_pin(DIO_LED_RED));
#ifdef DIO_LED_GREEN
    EXPECT_TRUE(test_gpio_pin(DIO_LED_GREEN));
#endif

#ifdef HAS_IO_BANG
    EXPECT_TRUE(test_gpio_pin(DIO_SCLK));
    EXPECT_TRUE(test_gpio_pin(DIO_MOSI));
    EXPECT_TRUE(test_gpio_pin(DIO_BATT_SCL));
    EXPECT_TRUE(test_gpio_pin(DIO_BATT_SDA));
    EXPECT_TRUE(test_gpio_pin(DIO_GNSS_INT));
    EXPECT_TRUE(test_gpio_pin(DIO_GNSS_RXD));
    EXPECT_TRUE(test_gpio_pin(DIO_GNSS_SAFEBOOT_N));
    EXPECT_TRUE(test_gpio_pin(DIO_PS_RS232));
    EXPECT_TRUE(test_gpio_pin(DIO_PWR_MUX_CTRL));
#endif
    return true;
}

#ifdef HAS_IO_BANG
bool test_gpio_write_mosi(void) {
    EXPECT_TRUE(test_gpio_pin(DIO_MOSI));
    return true;
}

bool test_gpio_write_sclk(void) {
    EXPECT_TRUE(test_gpio_pin(DIO_SCLK));
    return true;
}

bool test_gpio_write_batt_sda(void) {
    EXPECT_TRUE(test_gpio_pin(DIO_BATT_SDA));
    return true;
}

bool test_gpio_write_batt_scl(void) {
    EXPECT_TRUE(test_gpio_pin(DIO_BATT_SCL));
    return true;
}

bool test_gpio_write_pwr_mux_ctrl(void) {
    EXPECT_TRUE(test_gpio_pin(DIO_PWR_MUX_CTRL));
    return true;
}
#endif /*HAS_IO_BANG*/

#ifdef DIO_PS_RS232
bool test_gpio_write_ps_rs232(void) {
    EXPECT_TRUE(test_gpio_pin(DIO_PS_RS232));
    return true;
}
#endif

#ifdef DIO_GNSS_SAFEBOOT_N
bool test_gpio_write_gnss_safeboot_n(void) {
    EXPECT_TRUE(test_gpio_pin(DIO_GNSS_SAFEBOOT_N));
    return true;
}
#endif

#ifdef DIO_GNSS_RXD
bool test_gpio_write_gnss_rxd(void) {
    EXPECT_TRUE(test_gpio_pin(DIO_GNSS_RXD));
    return true;
}
#endif

#ifdef DIO_GNSS_INT
bool test_gpio_write_gnss_int(void) {
    EXPECT_TRUE(test_gpio_pin(DIO_GNSS_INT));
    return true;
}
#endif

#ifdef DIO_GNSS_RST_N
bool test_gpio_write_gnss_rst_n(void) {
    EXPECT_TRUE(test_gpio_pin(DIO_GNSS_RST_N));
    return true;
}
#endif

#ifdef DIO_LED_GREEN
bool test_gpio_write_led_green(void) {
    EXPECT_TRUE(test_gpio_pin(DIO_LED_GREEN));
    return true;
}
#endif

bool test_gpio_write_led_red(void) {
    EXPECT_TRUE(test_gpio_pin(DIO_LED_RED));
    return true;
}

#ifdef DIO_CAN_SS
bool test_gpio_write_can_ss(void) {
    EXPECT_TRUE(test_gpio_pin(DIO_CAN_SS));
    return true;
}
#endif

bool test_gpio_write_sx1262_ss(void) {
    EXPECT_TRUE(test_gpio_pin(DIO_SX1262_SS));
    return true;
}

bool test_gpio_write_sx1262_rst(void) {
    EXPECT_TRUE(test_gpio_pin(DIO_SX1262_RST));
    return true;
}

#ifdef DIO_CAN_RST
bool test_gpio_write_can_rst(void) {
    EXPECT_TRUE(test_gpio_pin(DIO_CAN_RST));
    return true;
}
#endif

#if 0
bool test_gpio_read(void) {

    return true;
}
#endif
