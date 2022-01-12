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


//zero-initialize the config structure.
gpio_config_t io_conf = {
//disable interrupt
.intr_type = GPIO_INTR_DISABLE,
//set as output mode
.mode = GPIO_MODE_OUTPUT,
//bit mask of the pins that you want to set,e.g.GPIO18/19
.pin_bit_mask = GPIO_SEL_4,
//disable pull-down mode
.pull_down_en = 0,
//disable pull-up mode
.pull_up_en = 0,
};

bool gpio_init(void) {
    bool res = true;
    gpio_config(&io_conf);
    gpio_intr_disable(GPIO_NUM_4);
	gpio_set_level(GPIO_NUM_4,(uint32_t) 0);
    return res;
}

bool gpio_get_state(uint8_t dio_number, uint8_t* logic_level) {
    bool res = false;
    if(logic_level) {
    	int value = gpio_get_level( dio_number);
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


PullMode_t gpio_get_pull_mode(uint8_t dio_pin) {
    PullMode_t pull_mode = PULL_UNDEF;

    return pull_mode;
}

bool gpio_get_in_mode(uint8_t dio_pin) {
    bool res = false;
    return res;
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
    const char* pin_name = "-";

    return pin_name;
}

uint8_t get_aux_num(uint8_t io_pin) {
    uint8_t aux_pin = 0;

    return aux_pin;
}

bool gpio_toggle(uint8_t dio_number) {
    bool res = false;
    static int cur_state=0;
	esp_err_t stat = ESP_OK;
	switch(cur_state ){
	case 0:
		stat = gpio_set_level((gpio_num_t)dio_number,(uint32_t) 1);
		cur_state =1;
		break;
	case 1:
		stat = gpio_set_level((gpio_num_t)dio_number,(uint32_t) 0);
		cur_state=0;
		break;
	}
	if(ESP_OK==stat){
		res = true;
	}
    return res;
}

uint32_t gpio_get_alter_fun(uint8_t dio_pin) {

    return 0;
}

DioDir_t gpio_get_dir(uint8_t dio_pin) {
    DioDir_t dir = GPIO_DIR_UNDEF;
  

    return dir;
}

bool gpio_set_dir(uint8_t dio_pin, DioDir_t des_dir) {
    bool res = false;
  
    return res;
}
