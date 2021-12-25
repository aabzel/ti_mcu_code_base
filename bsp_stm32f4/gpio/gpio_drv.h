#ifndef GPIO_DRV_H
#define GPIO_DRV_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "stm32f4xx_hal.h"

typedef enum uPort_t{
    PORT_A=0,
    PORT_B=1,
    PORT_C=2,
    PORT_D=3,
    PORT_E=4,
    PORT_F=5,
    PORT_G=6,
    PORT_H=7,
    PORT_UNDEF=8,
}Port_t;


typedef union uPad_t{
    struct {
        uint8_t port :4; /*A B C D E F G H*/
        uint8_t pin :4; /*0....15*/
    };
    uint8_t byte;
}Pad_t;

#define PIN_NAME_LEN 10
typedef struct xPin_t {
    Pad_t pad;
    char name[PIN_NAME_LEN];
    uint8_t mcu_pin;
    uint32_t mode; /* Specifies the operating mode for the selected pins.*/
    uint32_t pull; /* Specifies the Pull-up or Pull-Down activation for the selected pins.*/
    uint32_t speed; /*GPIO_speed_define*/
    uint32_t alternate; /*Peripheral to be connected to the selected pins.*/
    GPIO_PinState pin_state;
}Pin_t;

GPIO_TypeDef *Port2PortPtr(uint8_t port_num);
bool gpio_init(void);
uint8_t gpio_port_pin2pad(Port_t port, uint8_t pin);
uint32_t PinNum2PinMask(uint8_t pin_num);
Port_t PortLetter2PortNum(char port);
bool gpio_get_state(uint8_t io_pin, uint8_t* logic_level);
uint32_t gpio_read(uint8_t pad_num);
bool gpio_set_state(uint8_t io_pin, uint8_t logic_level);
bool is_edge_irq_en(uint8_t dio_pin);
bool gpio_toggle(uint8_t io_pin);
bool proc_led(void);
uint8_t get_mcu_pin(uint8_t io_pin);
uint8_t get_aux_num(uint8_t io_pin);
bool gpio_init_one(Pin_t *pinInstance);
bool gpios_init(void) ;
bool gpio_init(void);

#ifdef __cplusplus
}
#endif

#endif /* GPIO_DRV_H  */
