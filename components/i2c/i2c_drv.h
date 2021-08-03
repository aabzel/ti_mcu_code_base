#ifndef I2C_DRV_H
#define I2C_DRV_H

#include <stdbool.h>
#include <stdint.h>

bool i2c_init(void);
bool i2c_send(uint8_t i2c_num, uint8_t *array, uint16_t  array_len) ;
bool i2c_scan(uint8_t i2c_num, uint8_t *array, uint16_t  array_len, uint16_t  *ic_cnt) ;

#endif /* I2C_DRV_H  */
