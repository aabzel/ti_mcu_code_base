#ifndef GPIO_COMMON_H
#define GPIO_COMMON_H

#include <stdint.h>

typedef enum ePullMode_t {
	PULL_DOWN = 1,
	PULL_UP = 2,
	PULL_AIR = 3,
	PULL_UNDEF = 4,
    PULL_UP_DOWN=5}
PullMode_t;

typedef enum eDioDirr_t {
    GPIO_DIR_IN = 0,
    GPIO_DIR_OUT = 1,
    GPIO_DIR_INOUT = 2,
    GPIO_DIR_NONE = 4,
    GPIO_DIR_UNDEF = 5,
	GPIO_DIR_OUT_OD = 6,
	GPIO_DIR_INOUT_OD = 7,
} DioDir_t;

const char* gpio_get_name(uint8_t io_pin);
uint8_t get_mcu_pin(uint8_t io_pin);


#endif /* GPIO_COMMON_H */
