// Figure 23-7. Master Single TRANSMIT  1821
#include "i2c_drv.h"

#include <hw_i2c.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#ifdef DeviceFamily_CC26X2
#include <PowerCC26XX.h>
#include <i2c.h>
#include <ti/drivers/I2C.h>
#include <ti/drivers/i2c/I2CCC26XX.h>
#endif

#include "bit_utils.h"

I2C_Handle I2C0Handle = NULL;
I2C_Params params;
uint32_t i2c_it_cnt = 0;

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

static void transferCallback(I2C_Handle handle, I2C_Transaction* transac, bool result) { i2c_it_cnt++; }

bool i2c_init(void) {

    bool res = true;
    //  I2CMasterDisable(  I2C0_BASE);
    //  I2CMasterIntDisable(  I2C0_BASE);
    // I2CMasterIntClear(  I2C0_BASE);
    // I2CSlaveDisable(  I2C0_BASE);
    // I2CMasterInitExpClk( I2C0_BASE,  I2C_100kHz,false);
    // I2CMasterEnable(  I2C0_BASE);
#if 1
    I2C_init();
    I2C_Params_init(&params);
    params.bitRate = I2C_100kHz;
    params.transferMode = I2C_MODE_BLOCKING;
    params.transferCallbackFxn = transferCallback;
    I2C0Handle = I2C_open(0, &params);
    if(I2C0Handle) {
        res = true;
    }
#endif
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
        res = I2C_transfer(I2C0Handle, &transaction);
#ifdef I2C_BLOCKING
        int_fast16_t ret;
        ret = I2C_transferTimeout(I2C0Handle, &transaction, 100);
        if(I2C_STATUS_SUCCESS == ret) {
            res = true;
        }
#endif
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
#ifdef I2C_BLOCKING
        int_fast16_t ret;
        ret = I2C_transferTimeout(I2C0Handle, &transaction, 50);
        if(I2C_STATUS_SUCCESS == ret) {
            res = true;
        }
#endif
    }
    return res;
}

/*
 Figure 23-8. Master Single RECEIVE
 page 1822
 */

bool i2c_wait_busy(void) {
    bool res = true;

    uint32_t time_out_cnt = 0;
    while(I2CMasterBusy(I2C0_BASE)) {
        time_out_cnt++;
        if(0x002FFFFF < time_out_cnt) {
            res = false;
            break;
        }
    }
    return res;
}

bool i2c_check_addr(uint8_t i2c_addr) {
    bool res = true;

    I2CMasterSlaveAddrSet(I2C0_BASE, i2c_addr >> 1, true);

    //   res = wait_busy();

    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_SEND);

    res = i2c_wait_busy();

    if(res) {
        if(I2C_MSTAT_ADRACK_N_MASK == (I2C_MSTAT_ADRACK_N_MASK & HWREG(I2C0_BASE + I2C_O_MSTAT))) {
            res = false;
        } else {
            res = true;
        }
    }

    // I2CMasterControl(  I2C0_BASE,   I2C_MASTER_CMD_BURST_SEND_FINISH);

    return res;
}

bool i2c_send_signal(uint8_t signal) {
    volatile uint32_t* p_master_ctl_address = (uint32_t*)I2C_MCTRL_REG;
    bool res = true;
    switch(signal) {
    case 0:
        (*p_master_ctl_address) = (1 << 0);
        break;
    case 1:
        (*p_master_ctl_address) = (1 << 1);
        break;
    case 2:
        (*p_master_ctl_address) = (1 << 2);
        break;
    default:
        (*p_master_ctl_address) = signal;
        res = false;
        break;
    }
    return res;
}

bool i2c_scan(uint8_t i2c_num, uint8_t* array, uint16_t array_len, uint16_t* ic_cnt) { return false; }
