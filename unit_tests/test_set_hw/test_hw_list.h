#ifndef TEST_HARDWARE_LIST_H
#define TEST_HARDWARE_LIST_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#ifdef HAS_TEST_FLASH
#include "test_flash.h"
#else
#define TEST_SUIT_FLASH
#endif

#ifdef HAS_TEST_FLASH_FS
#include "test_flash_fs.h"
#else
#define TEST_SUIT_FLASH_FS
#endif

#ifdef HAS_TEST_PARAM
#include "test_params.h"
#else
#define TEST_SUIT_PARAM
#endif

#ifdef HAS_TEST_TCAN4550
#include "test_tcan4550.h"
#else
#define TEST_SUIT_TCAN4550
#endif

#ifdef HAS_TEST_GPIO
#include "test_gpio.h"
#else
#define TEST_SUIT_GPIO
#endif

#ifdef HAS_TEST_SPI
#include "test_spi.h"
#else
#define TEST_SUIT_SPI
#endif

#ifdef HAS_TEST_UART
#include "test_uart.h"
#else
#define TEST_SUIT_UART
#endif

#ifdef HAS_TEST_SX1262
#include "test_sx1262.h"
#else
#define TEST_SUIT_SX1262
#endif

#ifdef HAS_TEST_ZED_F9P
#include "test_zed_f9p.h"
#else
#define TEST_SUIT_ZED_F9P
#endif

#ifdef HAS_TEST_SYSTEM
#include "test_system.h"
#else
#define TEST_SYSTEM
#endif

bool test_clock_us(void);
bool test_clock_ms(void);

/*Compile time assemble array */
#define TEST_SUIT_HW \
    TEST_SUIT_PARAM \
	TEST_SUIT_FLASH    \
	TEST_SYSTEM \
	TEST_SUIT_UART \
	TEST_SUIT_GPIO \
	TEST_SUIT_ZED_F9P \
	TEST_SUIT_TCAN4550 \
	TEST_SUIT_FLASH_FS \
    TEST_SUIT_SPI \
	TEST_SUIT_SX1262 \
    {"clock_us", test_clock_us},\
    {"clock_ms", test_clock_ms},

#ifdef __cplusplus
}
#endif

#endif /*TEST_HARDWARE_LIST_H*/
