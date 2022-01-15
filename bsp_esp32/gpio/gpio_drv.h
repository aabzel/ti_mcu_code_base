#ifndef GPIO_DRV_H
#define GPIO_DRV_H

#include <stdbool.h>
#include <stdint.h>

#include "driver/gpio.h"
#include "gpio_common.h"

DioDir_t gpio_get_dir(uint8_t dio_pin);

bool gpio_set_pin_pull_mode(uint8_t dio_pin, PullMode_t pull_mode);
PullMode_t gpio_get_pull_mode(uint8_t dio_pin);
bool gpio_get_in_mode(uint8_t dio_pin);
bool gpio_get_state(uint8_t io_pin, uint8_t* logic_level);
bool gpio_init(void);
//bool gpio_init_layout(const Pin_t* PinTable, uint8_t size);
bool gpio_set_dir(uint8_t dio_pin, DioDir_t des_dir);
const char* gpio_get_name(uint8_t io_pin);
bool gpio_set_in_mode(uint8_t dio_pin, bool is_in_mode);
bool gpio_set_state(uint8_t io_pin, uint8_t logic_level);
bool gpio_toggle(uint8_t io_pin);
bool is_edge_irq_en(uint8_t dio_pin);

uint32_t gpio_get_alter_fun(uint8_t dio_pin);
uint8_t gpio_read(uint8_t dio_number);
uint8_t get_aux_num(uint8_t io_pin);
uint8_t get_mcu_pin(uint8_t io_pin);
#endif /* GPIO_DRV_H  */
