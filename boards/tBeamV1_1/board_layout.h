#ifndef BOARD_LAYOUT_H
#define BOARD_LAYOUT_H


#include "sys_config.h"

#define DIO_CNT 0
#define GPIO_COUNT 0U

#define PIN_NAME_LEN 10
typedef struct xPin_t {
    uint8_t dio;
    uint8_t mcu_pin;
    uint8_t aux_pin;
    char name[PIN_NAME_LEN];
} Pin_t;

extern const Pin_t PinTable[DIO_CNT];

#endif /* BOARD_LAYOUT_H  */
