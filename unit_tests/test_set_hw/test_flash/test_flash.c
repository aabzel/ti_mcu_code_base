#include "test_flash.h"

#include <stdbool.h>
#include <stdint.h>
#include <flash.h>
#include <string.h>
#include <vims.h>

#include "data_utils.h"
#include "flash_drv.h"
#include "io_utils.h"
#include "log.h"
#include "unit_test_check.h"

/*
tsr flash_write
*/
#define BLOCK_SIZE 129U
bool test_flash_write(void) {
    LOG_INFO(LG_FLASH,"0x%08x",FLASH_ADDR_TEST);
    EXPECT_TRUE( flash_erase_sector(FLASH_ADDR_TEST));
    EXPECT_TRUE(is_flash_spare(FLASH_ADDR_TEST, FLASH_SECTOR_SIZE));

    uint8_t DataRead[BLOCK_SIZE];
    memset(DataRead,0x00,sizeof(DataRead));

    uint8_t DataWrite[BLOCK_SIZE];
    memset(DataWrite,0x55,sizeof(DataWrite));

    EXPECT_TRUE(flash_wr(FLASH_ADDR_TEST, DataWrite,   BLOCK_SIZE));

    EXPECT_TRUE(flash_read(  FLASH_ADDR_TEST, DataRead, BLOCK_SIZE));
    memcpy(DataRead,(uint8_t*) FLASH_ADDR_TEST,sizeof(DataRead));

    EXPECT_EQ_MEM(DataRead, DataWrite,BLOCK_SIZE);

    EXPECT_TRUE( flash_erase_sector(FLASH_ADDR_TEST));
    EXPECT_TRUE(is_flash_spare(FLASH_ADDR_TEST, FLASH_SECTOR_SIZE));

    return true;
}


