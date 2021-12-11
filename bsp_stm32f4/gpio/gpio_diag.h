#ifndef GPIO_DIAG_H
#define GPIO_DIAG_H

#include <stdbool.h>
#include <stdint.h>

const char* GpioPort2str(uint8_t port);
const char* get_gpio_mode(uint8_t io_pin);
const char* get_gpio_type(uint8_t io_pin);
const char* get_pin_dir(uint8_t io_pin);
const char* get_gpio_alter_fun(uint8_t dio_pin);
const char* get_gpio_pull_mode(uint8_t dio_pin);
const char* get_gpio_edge(uint8_t dio_pin);

#endif /* GPIO_DIAG_H  */
