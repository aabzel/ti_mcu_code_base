#include "gpio_drv.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "driver/gpio.h"

#include "bit_utils.h"
#include "board_layout.h"
#include "data_utils.h"
#include "none_blocking_pause.h"


inline bool is_dio(uint8_t dio_pin) {
    bool res = false;

    return res;
}

bool gpio_init(void) {
    bool res = false;
 
    return res;
}

bool gpio_get_state(uint8_t dio_number, uint8_t* logic_level) {
    bool res = false;
    if(logic_level) {

        (*logic_level) = (uint8_t)value;

    }
    return res;
}

uint8_t gpio_read(uint8_t dio_number) { return 0x55; }

bool gpio_set_state(uint8_t dio_number, uint8_t logic_level) {
    gpio_set_level(dio_number, logic_level);
    return true;
}

bool is_edge_irq_en(uint8_t dio_pin) {
    bool res = false;

    return res;
}

bool gpio_set_pull_mode(uint8_t dio_pin, PullMode_t pull_mode) {
  

    return res;
}

PullMode_t gpio_get_pull_mode(uint8_t dio_pin) {
    PullMode_t pull_mode = PULL_UNDEF;

    return pull_mode;
}

bool gpio_get_in_mode(uint8_t dio_pin) {
    bool res = false;

    return is_input;
}

bool gpio_set_in_mode(uint8_t dio_pin, bool is_in_mode) {
    bool res = false;

    return res;
}

uint8_t get_mcu_pin(uint8_t io_pin) {
    uint8_t mcu_pin = 0;

    return mcu_pin;
}

const char* gpio_get_name(uint8_t io_pin) {
    const char* pin_name = 0;

    return pin_name;
}

uint8_t get_aux_num(uint8_t io_pin) {
    uint8_t aux_pin = 0;

    return aux_pin;
}

bool gpio_toggle(uint8_t dio_number) {
    bool res = false;

    return res;
}

uint32_t gpio_get_alter_fun(uint8_t dio_pin) {

    return port_id;
}

DioDir_t gpio_get_dir(uint8_t dio_pin) {
    DioDir_t dir = GPIO_DIR_UNDEF;
  

    return dir;
}

bool gpio_set_dir(uint8_t dio_pin, DioDir_t des_dir) {
    bool res = false;
  
    return res;
}
