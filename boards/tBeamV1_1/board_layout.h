#ifndef BOARD_LAYOUT_H
#define BOARD_LAYOUT_H

#include "sys_config.h"

#include "gpio_common.h"

#define DIO_CNT 49

#define PIN_NAME_LEN 10
typedef struct xPin_t {
    int16_t dio;
    uint8_t mcu_pin;
    DioDir_t dir;
    char name[PIN_NAME_LEN];
} Pin_t;

extern const Pin_t PinTable[DIO_CNT];

#endif /* BOARD_LAYOUT_H  */
