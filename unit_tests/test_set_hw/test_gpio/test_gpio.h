#ifndef TEST_GPIO_H
#define TEST_GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

bool test_gpio_write(void);

bool test_gpio_write_can_rst(void);
bool test_gpio_write_can_ss(void);
bool test_gpio_write_gnss_int(void);
bool test_gpio_write_gnss_rst_n(void);
bool test_gpio_write_gnss_safeboot_n(void);
bool test_gpio_write_led_green(void);
bool test_gpio_write_led_red(void);
bool test_gpio_write_ps_rs232(void);
bool test_gpio_write_sx1262_rst(void);
bool test_gpio_write_sx1262_ss(void);


#define TEST_SUIT_GPIO_GENERAL                         \
 /**/       {"gpio_wr_can_rst", test_gpio_write_can_rst},  \
 /**/       {"gpio_wr_can_ss", test_gpio_write_can_ss},     \
 /**/       {"gpio_wr_gnss_int", test_gpio_write_gnss_int},\
 /**/       {"gpio_wr_gnss_rst_n", test_gpio_write_gnss_rst_n},          \
 /**/       {"gpio_wr_gnss_safeboot", test_gpio_write_gnss_safeboot_n},    \
 /**/       {"gpio_wr_led_green", test_gpio_write_led_green},              \
 /**/       {"gpio_wr_led_red", test_gpio_write_led_red}, \
 /**/       {"gpio_wr_ps_rs232", test_gpio_write_ps_rs232}, \
 /**/       {"gpio_wr_sx1262_rst", test_gpio_write_sx1262_rst}, \
 /**/       {"gpio_wr_sx1262_ss", test_gpio_write_sx1262_ss},


#ifdef HAS_IO_BANG
bool test_gpio_write_batt_scl(void);
bool test_gpio_write_batt_sda(void);
bool test_gpio_write_gnss_rxd(void);
bool test_gpio_write_mosi(void);
bool test_gpio_write_pwr_mux_ctrl(void);
bool test_gpio_write_sclk(void);

#define TEST_SUIT_GPIO_IO_BANG                      \
/*1*/    {"gpio_wr_sclk", test_gpio_write_sclk},    \
/*2*/    {"gpio_wr_mosi", test_gpio_write_mosi},     \
/*3*/    {"gpio_wr_pwr_mux_ctrl", test_gpio_write_pwr_mux_ctrl}, \
/*4*/    {"gpio_wr_gnss_rxd", test_gpio_write_gnss_rxd},            \
/*5*/    {"gpio_wr_scl", test_gpio_write_batt_scl},                          \
/*6*/    {"gpio_wr_sda", test_gpio_write_batt_sda},

#else
#define TEST_SUIT_GPIO_IO_BANG
#endif /*HAS_IO_BANG*/

// bool test_gpio_read(void) ;

#define TEST_SUIT_GPIO                                                                                                 \
    TEST_SUIT_GPIO_GENERAL                                                                                             \
    TEST_SUIT_GPIO_IO_BANG                                                                                             \
    {"gpio_write", test_gpio_write},

//{"gpio_read", test_gpio_read},

#ifdef __cplusplus
}
#endif

#endif /* TEST_GPIO_H */
