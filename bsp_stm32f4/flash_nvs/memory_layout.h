
#ifndef NVS_MEMORY_LAYOUT_H
#define NVS_MEMORY_LAYOUT_H

#include "flash_drv.h"

#define FLASH_SECTOR_SIZE (128*K_BYTES)
#define FLASH_FS_PAGE_SIZE  (FLASH_SECTOR_SIZE)
#define NVS_SIZE (2*FLASH_FS_PAGE_SIZE) /*Must be double multiple of FLASH_SECTOR_SIZE*/
#define NVS_SIZE_PAGES (NVS_SIZE/FLASH_SECTOR_SIZE)
/* We should not touch the last flash memory sector.
 *
 * The last flash memory sector must contain a Customer Configuration section (CCFG)
 * that is used by boot ROM and TI provided drivers to configure the device.*/
#define NVS_FLASH_START (0x08040000)
#define NVS_FLASH_END   (NVS_FLASH_START+NVS_SIZE-1)
/* page 1 */
#define MEMORY_MANAGER1_OFFSET	NVS_FLASH_START
#define MEMORY_MANAGER1_LENGTH	FLASH_FS_PAGE_SIZE

/* page 2 */
#define MEMORY_MANAGER2_OFFSET	(NVS_FLASH_START+FLASH_FS_PAGE_SIZE)
#define MEMORY_MANAGER2_LENGTH	FLASH_FS_PAGE_SIZE

#endif /* NVS_MEMORY_LAYOUT_H */
