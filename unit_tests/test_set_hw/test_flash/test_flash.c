#include "test_flash.h"

#include <stdbool.h>
#include <stdint.h>
#include <flash.h>
#include <string.h>
#include <vims.h>

#include "data_utils.h"
#include "flash_drv.h"
#include "flash_fs.h"
#include "io_utils.h"
#include "log.h"
#include "unit_test_check.h"

/*
tsr flash_write
*/
#define BLOCK_SIZE 129U

bool test_flash_write(void) {
    uint32_t spare_page = flash_get_first_spare_page();
    EXPECT_NE(0xFFFFFFFF, spare_page);
    LOG_INFO(LG_FLASH,"0x%08x",spare_page);
    spare_page = FLASH_ADDR_TEST;
    EXPECT_TRUE(is_flash_spare(  spare_page, FLASH_SECTOR_SIZE ));

    EXPECT_FALSE(is_flash_fs_addr(spare_page));

    EXPECT_TRUE( flash_erase_sector(spare_page));
    EXPECT_TRUE(is_flash_spare(spare_page, FLASH_SECTOR_SIZE));

    uint8_t DataRead[BLOCK_SIZE];
    memset(DataRead,0x00,sizeof(DataRead));

    uint8_t DataWrite[BLOCK_SIZE];
    memset(DataWrite,0x55,sizeof(DataWrite));

    EXPECT_TRUE(flash_wr(spare_page, DataWrite,   BLOCK_SIZE));

    EXPECT_TRUE(flash_read(  spare_page, DataRead, BLOCK_SIZE));
    memcpy(DataRead,(uint8_t*) spare_page,sizeof(DataRead));

    EXPECT_EQ_MEM(DataRead, DataWrite,BLOCK_SIZE);

    EXPECT_TRUE( flash_erase_sector(spare_page));
    EXPECT_TRUE(is_flash_spare(spare_page, FLASH_SECTOR_SIZE));

    return true;
}


