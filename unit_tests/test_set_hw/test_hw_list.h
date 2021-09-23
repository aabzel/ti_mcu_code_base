#ifndef TEST_HARDWARE_LIST_H
#define TEST_HARDWARE_LIST_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "test_flash.h"
#include "test_flash_fs.h"
#include "test_params.h"
#include "test_spi.h"
#include "test_uart.h"
#include "test_sx1262.h"

bool test_clock_us(void);
bool test_clock_ms(void);

/*Compile time assemble array */
#define TEST_SUIT_HW \
    TEST_SUIT_PARAMS \
	TEST_SUIT_FLASH    \
	TEST_SUIT_UART \
	TEST_SUIT_FLASH_FS \
    TEST_SUIT_SPI \
	TEST_SUIT_SX1262 \
    {"clock_us", test_clock_us},\
    {"clock_ms", test_clock_ms},

#ifdef __cplusplus
}
#endif

#endif /*TEST_HARDWARE_LIST_H*/
