#ifndef TEST_GPIO_H
#define TEST_GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#include "sys_config.h"

bool test_gpio_write(void);

#ifdef DIO_CAN_RST
bool test_gpio_write_can_rst(void);
#define TEST_SUIT_GPIO_CAN_RST {"gpio_wr_can_rst", test_gpio_write_can_rst},
#else
#define TEST_SUIT_GPIO_CAN_RST
#endif

#ifdef DIO_CAN_SS
bool test_gpio_write_can_ss(void);
#define TEST_SUIT_GPIO_CAN_SS {"gpio_wr_can_ss", test_gpio_write_can_ss},
#else
#define TEST_SUIT_GPIO_CAN_SS
#endif

#ifdef DIO_GNSS_RST_N
bool test_gpio_write_gnss_rst_n(void);
#define TEST_SUIT_GPIO_GNSS_RST {"gpio_wr_gnss_rst_n", test_gpio_write_gnss_rst_n},
#else
#define TEST_SUIT_GPIO_GNSS_RST
#endif

#ifdef DIO_GNSS_INT
bool test_gpio_write_gnss_int(void);
#define TEST_SUIT_GPIO_GNSS_INT {"gpio_wr_gnss_int", test_gpio_write_gnss_int},
#else
#define TEST_SUIT_GPIO_GNSS_INT
#endif

#ifdef DIO_GNSS_SAFEBOOT_N
bool test_gpio_write_gnss_safeboot_n(void);
#define TEST_SUIT_GPIO_GNSSSAFEBOOT {"gpio_wr_gnss_safeboot", test_gpio_write_gnss_safeboot_n},
#else
#define TEST_SUIT_GPIO_GNSSSAFEBOOT
#endif

#ifdef DIO_SX1262_RST
bool test_gpio_write_sx1262_rst(void);
#define TEST_SUIT_GPIO_SX1262_RST {"gpio_wr_sx1262_rst", test_gpio_write_sx1262_rst},
#else
#define TEST_SUIT_GPIO_SX1262_RST
#endif

#ifdef DIO_SX1262_SS
bool test_gpio_write_sx1262_ss(void);
#define TEST_SUIT_GPIO_SX1262_SS {"gpio_wr_sx1262_ss", test_gpio_write_sx1262_ss},
#else
#define TEST_SUIT_GPIO_SX1262_SS
#endif

#ifdef DIO_LED_GREEN
bool test_gpio_write_led_green(void);
#define TEST_SUIT_GPIO_LED_GREEN {"gpio_wr_led_green", test_gpio_write_led_green},
#else
#define TEST_SUIT_GPIO_LED_GREEN
#endif

#ifdef DIO_LED_RED
bool test_gpio_write_led_red(void);
#define TEST_SUIT_GPIO_LED_RED {"gpio_wr_led_red", test_gpio_write_led_red},
#else
#define TEST_SUIT_GPIO_LED_RED
#endif

#ifdef DIO_PS_RS232
bool test_gpio_write_ps_rs232(void);
#define TEST_SUIT_GPIO_RS232 {"gpio_wr_ps_rs232", test_gpio_write_ps_rs232},
#else
#define TEST_SUIT_GPIO_RS232
#endif

#ifdef DIO_BATT_SCL
bool test_gpio_write_batt_scl(void);
#define TEST_SUIT_GPIO_BATT_SCL {"gpio_wr_scl", test_gpio_write_batt_scl},
#else
#define TEST_SUIT_GPIO_BATT_SCL
#endif /*HAS_IO_BANG*/

#ifdef DIO_BATT_SDA
bool test_gpio_write_batt_sda(void);
#define TEST_SUIT_BATT_SDA {"gpio_wr_sda", test_gpio_write_batt_sda},
#else
#define TEST_SUIT_BATT_SDA
#endif

#ifdef DIO_MOSI
bool test_gpio_write_mosi(void);
#define TEST_SUIT_MOSI {"gpio_wr_mosi", test_gpio_write_mosi},
#else
#define TEST_SUIT_MOSI
#endif

#ifdef DIO_SCLK
bool test_gpio_write_sclk(void);
#define TEST_SUIT_SCLK {"gpio_wr_sclk", test_gpio_write_sclk},
#else
#define TEST_SUIT_SCLK
#endif

#ifdef DIO_PWR_MUX_CTRL
bool test_gpio_write_pwr_mux_ctrl(void);
#define TEST_SUIT_PWR_MUX_CTRL {"gpio_wr_pwr_mux_ctrl", test_gpio_write_pwr_mux_ctrl},
#else
#define TEST_SUIT_PWR_MUX_CTRL
#endif

#ifdef DIO_GNSS_RXD
bool test_gpio_write_gnss_rxd(void);
#define TEST_SUIT_GNSS_RXD {"gpio_wr_gnss_rxd", test_gpio_write_gnss_rxd},
#else
#define TEST_SUIT_GNSS_RXD
#endif /*DIO_GNSS_RXD*/

// bool test_gpio_read(void) ;

#define TEST_SUIT_GPIO                                                                                                 \
    TEST_SUIT_GPIO_LED_RED                                                                                             \
    TEST_SUIT_GNSS_RXD                                                                                                 \
    TEST_SUIT_PWR_MUX_CTRL                                                                                             \
    TEST_SUIT_MOSI                                                                                                     \
    TEST_SUIT_GPIO_RS232                                                                                               \
    TEST_SUIT_GPIO_CAN_RST                                                                                             \
    TEST_SUIT_GPIO_BATT_SCL                                                                                            \
    TEST_SUIT_BATT_SDA                                                                                                 \
    TEST_SUIT_GPIO_CAN_SS                                                                                              \
    TEST_SUIT_GPIO_GNSS_INT                                                                                            \
    TEST_SUIT_GPIO_GNSS_RST                                                                                            \
    TEST_SUIT_GPIO_GNSSSAFEBOOT                                                                                        \
    TEST_SUIT_SCLK                                                                                                     \
    TEST_SUIT_GPIO_SX1262_SS                                                                                           \
    TEST_SUIT_GPIO_SX1262_RST                                                                                          \
    TEST_SUIT_GPIO_LED_GREEN{"gpio_write", test_gpio_write},

//{"gpio_read", test_gpio_read},

#ifdef __cplusplus
}
#endif

#endif /* TEST_GPIO_H */
