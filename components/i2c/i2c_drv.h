#ifndef I2C_DRV_H
#define I2C_DRV_H

#include <hw_ints.h>
#include <hw_memmap.h>
#include <ioc.h>
#include <stdbool.h>
#include <stdint.h>
/*!
 *  @def    BOARD_I2CName
 *  @brief  Enum of I2C names
 */
typedef enum BOARD_I2CName {
    BOARD_I2C0 = 0,

    BOARD_I2CCOUNT
} BOARD_I2CName;

/* I2C */
#define BOARD_I2C0_SCL0 IOID_4
#define BOARD_I2C0_SDA0 IOID_5

bool i2c_init(void);
bool i2c_send(uint8_t i2c_num, uint8_t* array, uint16_t array_len);
bool i2c_scan(uint8_t i2c_num, uint8_t* array, uint16_t array_len, uint16_t* ic_cnt);

#endif /* I2C_DRV_H  */
