/*
 https://software-dl.ti.com/dsps/dsps_public_sw/sdo_sb/targetcontent/tirtos/2_20_00_06/exports/tirtos_full_2_20_00_06/products/tidrivers_full_2_20_00_08/docs/doxygen/html/_i2_c_8h.html
*/
#ifndef I2C_DRV_H
#define I2C_DRV_H

//#include <hw_ints.h>
//#include <hw_memmap.h>
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

#define I2C_MSTAT_BUSY_BIT 0
#define I2C_MSTAT_BUSY_MASK 0x00000001
#define I2C_MSTAT_ADRACK_N_MASK 0x00000004

#define I2C_MSA_REG (I2C0_BASE + 0x800)
#define I2C_MSTAT_REG (I2C0_BASE + 0x804) /*     1839*/
#define I2C_MCTRL_REG (I2C0_BASE + 0x804) /*page 1841*/
#define I2C_MDR_REG (I2C0_BASE + 0x808)
#define I2C_MTPR_REG (I2C0_BASE + 0x80C)
#define I2C_MCR_REG (I2C0_BASE + 0x820)

extern uint32_t i2c_it_cnt;

bool i2c_init(void);
bool i2c_read(uint8_t i2c_num, uint8_t i2c_addr, uint8_t* array, uint16_t array_len);
bool i2c_write(uint8_t i2c_num, uint8_t i2c_addr, uint8_t* array, uint16_t array_len);
bool i2c_scan(uint8_t i2c_num, uint8_t* array, uint16_t array_len, uint16_t* ic_cnt);
bool i2c_check_addr(uint8_t addr);
bool i2c_send_signal(uint8_t signal);
bool i2c_wait_busy(void);
#endif /* I2C_DRV_H  */
