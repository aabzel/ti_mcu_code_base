#include "test_tcan4550.h"

#include <string.h>
#include <gpio.h>

#include "tcan4550_diag.h"
#include "tcan4550_drv.h"
#include "unit_test_check.h"

bool test_can_detect(void) {
    EXPECT_TRUE( is_tcan4550_connected());
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
    EXPECT_EQ(4, sizeof(HeaderCom_t));
    EXPECT_EQ(4, sizeof(tCanRegRev_t));
    EXPECT_EQ(4, sizeof(tCanRegStatus_t));
    EXPECT_EQ(4, sizeof(tCanRegModeOpPinCfg_t));
    EXPECT_EQ(4, sizeof(TxBufCfg_t));
    EXPECT_EQ(4, sizeof(TxBufElmSzCfg_t));
    EXPECT_EQ(4, sizeof(W0_t));
    EXPECT_EQ(4, sizeof(TxBuffW1_t));

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
    for(addr=ADDR_MRAM; addr<=(ADDR_MRAM+MRAM_SIZE-4); addr++){
        EXPECT_TRUE( test_can_address(  addr ));
    }
    return true;
}
