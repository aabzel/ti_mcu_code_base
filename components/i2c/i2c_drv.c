#include "i2c_drv.h"

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#ifdef DeviceFamily_CC26X2
#endif

bool i2c_init(void) {
    bool res = false;
    return res;
}

bool i2c_send(uint8_t i2c_num, uint8_t *array, uint16_t  array_len) {
    return false;
}

bool i2c_scan(uint8_t i2c_num, uint8_t *array, uint16_t  array_len, uint16_t  *ic_cnt) {
    return false;
}
