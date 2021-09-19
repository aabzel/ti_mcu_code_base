#ifndef GPIO_DRV_H
#define GPIO_DRV_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
  
#include "board_layout.h"
#define GPIO_LED_ON (1)
#define GPIO_LED_OFF (0)

bool gpio_init(void);

bool gpio_get_state(uint8_t io_pin, uint8_t* logic_level);
bool gpio_set_state(uint8_t io_pin, uint8_t logic_level);
bool is_edge_irq_en(uint8_t dio_pin);
bool gpio_toggle(uint8_t io_pin);
bool proc_led(void);
uint8_t get_mcu_pin(uint8_t io_pin);
uint8_t get_aux_num(uint8_t io_pin);


#ifdef __cplusplus
}
#endif

#endif /* GPIO_DRV_H  */
