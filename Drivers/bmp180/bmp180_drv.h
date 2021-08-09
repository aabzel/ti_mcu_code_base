/*
https://github.com/reaper7/esp8266_i2c_bmp180/blob/master/i2c_bmp180.h
 *
 * */

#ifndef BMP180_DRV_H
#define BMP180_DRV_H

#include <stdbool.h>
#include <stdint.h>

#define BMP180_ADDR_W   0xEE
#define BMP180_ADDR_R   0xEF

#define BMP180_CTRL_REG  0xf4
#define BMP180_DATA_REG 0xf6
#define BMP180_CHIP_ID_REG            (0xD0)


bool bmp180_read_reg(uint8_t reg_addr, uint8_t *reg_val);

#endif /* BMP180_DRV_H  */
