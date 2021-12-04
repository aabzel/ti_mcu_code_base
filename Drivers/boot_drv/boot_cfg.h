#ifndef BOOT_CFG_H
#define BOOT_CFG_H

#include "flash_drv.h"
#include "sys.h"
#include "memory_layout.h"

//#define APP_START_ADDRESS 0x0001A000
#define APP_PAGE_START (APP_START_ADDRESS/FLASH_SECTOR_SIZE)
#define FLASH_PAGE_BOOT_CNT APP_PAGE_START
#define APP_PAGE_CNT (FLASH_PAGE_CNT-FLASH_PAGE_BOOT_CNT-NVS_SIZE_PAGES-1)
#define APP_LAYNCH_TRY 5U

#endif /* BOOT_CFG_H */
