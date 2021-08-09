#include "bmp180_drv.h"

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <i2c.h>

#ifdef DeviceFamily_CC26X2

#endif
#include "i2c_drv.h"

bool bmp180_read_reg(uint8_t reg_addr, uint8_t *reg_val){
    bool res = true;
    I2CMasterSlaveAddrSet( I2C0_BASE,  BMP180_ADDR_R>>1, true);
    I2CMasterControl(  I2C0_BASE,   I2C_MASTER_CMD_SINGLE_SEND);
    res=i2c_wait_busy()&&res;

    I2CMasterDataPut(  I2C0_BASE,   reg_addr);
    I2CMasterControl(  I2C0_BASE,   I2C_MASTER_CMD_BURST_SEND_START);
    res=i2c_wait_busy()&&res;

    I2CMasterControl(  I2C0_BASE,   I2C_MASTER_CMD_BURST_RECEIVE_START );
    res=i2c_wait_busy()&&res;

    *reg_val=I2CMasterDataGet(  I2C0_BASE);
    I2CMasterControl(  I2C0_BASE,   I2C_MASTER_CMD_BURST_SEND_FINISH  );
#if 0
    uint8_t array[2];
    array[0]=reg_addr;
    array[1]=0xFF;
    res = i2c_read(0, (uint8_t)BMP180_ADDR_R, array, 2);
    if(res){
        *reg_val = array[1];
    }
#endif
    return res;
}
