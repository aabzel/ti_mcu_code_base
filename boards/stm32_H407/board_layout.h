#ifndef BOARD_LAYOUT_H
#define BOARD_LAYOUT_H

#include <stdint.h>

#include "sys_config.h"
#include "stm32f4xx_hal.h"

#ifndef USE_HAL_DRIVER
#error "that wile only for STM32 MCUs"
#endif /*USE_HAL_DRIVER*/

typedef enum uPort_t{
    PORT_A=0,
    PORT_B=1,
    PORT_C=2,
    PORT_D=3,
    PORT_E=4,
    PORT_F=5,
    PORT_G=6,
    PORT_H=6,
}Port_t;

typedef struct xPad_t{
    uint8_t port :4; /*A B C D E F G H*/
    uint8_t pin :4; /*0....15*/
}Pad_t;

#define PIN_NAME_LEN 10
typedef struct xPin_t {
    Pad_t pad;
    char name[PIN_NAME_LEN];
    uint8_t mcu_pin;
    uint32_t mode; /* Specifies the operating mode for the selected pins.*/
    uint32_t Pull; /* Specifies the Pull-up or Pull-Down activation for the selected pins.*/
    uint32_t Speed; /*GPIO_speed_define*/
    uint32_t Alternate; /*Peripheral to be connected to the selected pins.*/
    GPIO_PinState pin_state;
}Pin_t;

extern Pin_t PinTable[];

bool gpio_init(void);
uint32_t gpio_get_cnt(void);

#endif /* BOARD_LAYOUT_H  */
