#include "test_tcan4550.h"

#include <string.h>
#include <gpio.h>

#include "TCAN_hl.h"
#include "tcan4550_diag.h"
#include "tcan4550_drv.h"
#include "unit_test_check.h"

bool test_can_detect(void) {
    EXPECT_TRUE( is_tcan4550_connected());
    return true;
}

bool test_can_lock(void){
    tCanRegCCctrl_t ctrl_reg;
    ctrl_reg.word =0;

    EXPECT_TRUE( tcan4550_set_lock(true));
    EXPECT_TRUE( is_tcan4550_protected_reg_locked(&ctrl_reg));
    EXPECT_FALSE(is_tcan4550_protected_reg_unlock(&ctrl_reg));

    EXPECT_TRUE( tcan4550_set_lock(false));
    EXPECT_FALSE( is_tcan4550_protected_reg_locked(&ctrl_reg));
    EXPECT_TRUE(is_tcan4550_protected_reg_unlock(&ctrl_reg));
    return true;
}

static bool test_can_write_one(uint16_t addr, uint32_t write_val) {
    uint32_t read_reg=0;
    EXPECT_TRUE( tcan4550_write_reg(addr, write_val)) ;
    EXPECT_TRUE( tcan4550_read_reg(addr, &read_reg)) ;
    EXPECT_EQ(write_val, read_reg);
    return true;
}



bool test_can_write(void) {
    EXPECT_TRUE(test_can_write_one(ADDR_READ_WRITE_TEST,0x55555555));
    EXPECT_TRUE(test_can_write_one(ADDR_READ_WRITE_TEST,0xAAAAAAAA));
    EXPECT_TRUE(test_can_write_one(ADDR_READ_WRITE_TEST,0x12345678));
    EXPECT_TRUE(test_can_write_one(ADDR_READ_WRITE_TEST,0x87654321));
    return true;
}

bool test_can_read_word(void) {
    uint32_t read_dev_id0_4 = 0;
    EXPECT_TRUE( tcan4550_read(ADDR_DEVICE_ID0, 1, (uint8_t *)&read_dev_id0_4, sizeof(read_dev_id0_4)));
    EXPECT_EQ(0x5443414E, read_dev_id0_4);
    return true;
}

bool test_can_types(void) {
    EXPECT_EQ(4, sizeof(tCanRegRev_t));
    EXPECT_EQ(4, sizeof(tCanRegStatus_t));
    EXPECT_EQ(4, sizeof(tCanRegModeOpPinCfg_t));
    EXPECT_EQ(4, sizeof(tCanRegTxBufCfg_t));
    EXPECT_EQ(4, sizeof(tCanRegFiFo0Stat_t));
    EXPECT_EQ(4, sizeof(tCanRegFiFo1Stat_t));
    EXPECT_EQ(4, sizeof(tCanRegTxBufElmSzCfg_t));
    EXPECT_EQ(4, sizeof(tCanRegCCctrl_t));
    EXPECT_EQ(4, sizeof(tCanRegIntEn_t));
    EXPECT_EQ(4, sizeof(tCanRegBitTime_t));
    EXPECT_EQ(4, sizeof(tCanRegDataBitTime_t));
    EXPECT_EQ(4, sizeof(tCanRegGloFiltCfg_t));
    EXPECT_EQ(4, sizeof(tCanRegStdIdFiltCfg_t));
    EXPECT_EQ(4, sizeof(tCanRegRxBufFiFoElemSzCgf_t));
    EXPECT_EQ(4, sizeof(tCanRegProtStat_t));
    EXPECT_EQ(4, sizeof(tCanRegExtIdFiltCfg_t));
    EXPECT_EQ(4, sizeof(tCanRegRxFifo0Cfg_t));
    EXPECT_EQ(4, sizeof(tCanRegTxEventFifoCgf_t));
    EXPECT_EQ(4, sizeof(tCanRegRxFifo1Cfg_t));
    EXPECT_EQ(4, sizeof(tCanRegRxBufCgf_t));
    EXPECT_EQ(4, sizeof(tCanRegIntLine_t));

    EXPECT_EQ(4, sizeof(tCan4550SidFilter_t));
    EXPECT_EQ(4, sizeof(TxBuffW1_t));
    EXPECT_EQ(4, sizeof(HeaderCom_t));
    EXPECT_EQ(4, sizeof(W0_t));

    return true;
}

static bool test_can_set_bitrate_one(uint32_t bit_rate){
    uint32_t read_bit_rate = 0.0;
    EXPECT_TRUE( tcan4550_set_bit_rate(  bit_rate));
    read_bit_rate = tcan4550_get_bit_rate();
    EXPECT_EQ(bit_rate,read_bit_rate);
    return true;
}

bool test_can_rate(void){
    uint32_t orig_bit_rate;
    orig_bit_rate = tcan4550_get_bit_rate();
    LOG_INFO(CAN,"Orig bit rate %u",orig_bit_rate );
    EXPECT_TRUE(test_can_set_bitrate_one(10000));
    EXPECT_TRUE(test_can_set_bitrate_one(20000));
    EXPECT_TRUE(test_can_set_bitrate_one(50000));
    EXPECT_TRUE(test_can_set_bitrate_one(125000));
    EXPECT_TRUE(test_can_set_bitrate_one(250000));
    EXPECT_TRUE(test_can_set_bitrate_one(500000));
#ifdef SET_800K_UNDEF_BUG
    EXPECT_TRUE(test_can_set_bitrate_one(800000));
#endif
    EXPECT_TRUE(test_can_set_bitrate_one(1000000));
    LOG_WARNING(CAN,"Set default bit rate %u",CAN_BAUD_RATE_DFLT );
    EXPECT_TRUE( tcan4550_set_bit_rate(  CAN_BAUD_RATE_DFLT));
    return true;
}

static bool test_can_address(uint16_t addr) {
    uint32_t orig_reg=0;
    EXPECT_TRUE( tcan4550_read_reg(addr, &orig_reg)) ;

    EXPECT_TRUE(test_can_write_one(addr,0xAAAAAAAA));
    EXPECT_TRUE(test_can_write_one(addr,0x55555555));
    EXPECT_TRUE(test_can_write_one(addr,0x12345678));

    EXPECT_TRUE( tcan4550_write_reg(addr, orig_reg)) ;
    return true;
}

bool test_can_mram(void){
    uint16_t addr=0;
    for(addr=ADDR_MRAM; addr<=(ADDR_MRAM+MRAM_SZ-4); addr+=5){
        EXPECT_TRUE( test_can_address(  addr ));
    }
    return true;
}

bool test_can_send(void){
    uint16_t id= 0x123;
    uint64_t data64 = 0x112233445566;
    EXPECT_TRUE( init_tcan());
    EXPECT_TRUE( tcan4550_send(id, data64));
    return true;
}
