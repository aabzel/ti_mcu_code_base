#include "gpio_common.h"

#include <stdint.h>

#include "board_layout.h"
#include "data_utils.h"

uint8_t get_mcu_pin(uint8_t io_pin) {
    uint8_t mcu_pin = 0;
    uint8_t i = 0;
    for(i = 0; i < pin_get_cnt(); i++) {
        if(PinTable[i].dio == io_pin) {
            mcu_pin = PinTable[i].mcu_pin;
            break;
        }
    }
    return mcu_pin;
}


const char* gpio_get_name(uint8_t io_pin) {
    const char* pin_name = 0;
    uint8_t i = 0;
    for(i = 0; i < pin_get_cnt(); i++) {
        if(PinTable[i].dio == io_pin) {
            pin_name = PinTable[i].name;
            break;
        }
    }
    return pin_name;
}


