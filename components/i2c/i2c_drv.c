#include "i2c_drv.h"

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#ifdef DeviceFamily_CC26X2
#include <PowerCC26XX.h>
#include <ti/drivers/I2C.h>
#include <ti/drivers/i2c/I2CCC26XX.h>
#endif

I2C_Handle I2C0Handle = NULL;

I2CCC26XX_Object i2cCC26xxObjects[BOARD_I2CCOUNT];

const I2CCC26XX_HWAttrsV1 i2cCC26xxHWAttrs[BOARD_I2CCOUNT] = {{
    .baseAddr = I2C0_BASE,
    .powerMngrId = PowerCC26XX_PERIPH_I2C0,
    .intNum = INT_I2C_IRQ,
    .intPriority = ~0,
    .swiPriority = 0,
    .sdaPin = BOARD_I2C0_SDA0,
    .sclPin = BOARD_I2C0_SCL0,
}};

const I2C_Config I2C_config[BOARD_I2CCOUNT] = {
    {// .fxnTablePtr = &I2CCC26XX_fxnTable,
     .object = &i2cCC26xxObjects[BOARD_I2C0],
     .hwAttrs = &i2cCC26xxHWAttrs[BOARD_I2C0]},
};

const uint_least8_t I2C_count = BOARD_I2CCOUNT;

bool i2c_init(void) {
    bool res = false;
    I2C_init();
    I2C_Params params;
    I2C_Params_init(&params);
    params.bitRate = I2C_400kHz;
    params.transferMode = I2C_MODE_BLOCKING;
    params.transferCallbackFxn = NULL;
    I2C0Handle = I2C_open(0, &params);
    if(I2C0Handle) {
        res = true;
    }
    return res;
}

bool i2c_write(uint8_t i2c_num, uint8_t i2c_addr, uint8_t* array, uint16_t array_len) {
    bool res = false;
    I2C_Transaction transaction = {0};
    transaction.slaveAddress = i2c_addr;
    transaction.writeBuf = array;
    transaction.writeCount = array_len;
    transaction.readCount = 0;
    transaction.readBuf = NULL;
    if(0 == i2c_num) {
        int_fast16_t ret;
        ret = I2C_transferTimeout(I2C0Handle, &transaction, 5000);
        if(I2C_STATUS_SUCCESS == ret) {
            res = true;
        }
    }
    return res;
}

bool i2c_read(uint8_t i2c_num, uint8_t i2c_addr, uint8_t* array, uint16_t array_len) {
    bool res = false;
    I2C_Transaction transaction = {0};
    transaction.slaveAddress = i2c_addr;

    // Read from I2C slave device
    transaction.readBuf = array;
    transaction.readCount = array_len;
    transaction.writeBuf = NULL;
    transaction.writeCount = 0;
    if(0 == i2c_num) {
        res = I2C_transfer(I2C0Handle, &transaction);
    }
    return res;
}

bool i2c_scan(uint8_t i2c_num, uint8_t* array, uint16_t array_len, uint16_t* ic_cnt) { return false; }
