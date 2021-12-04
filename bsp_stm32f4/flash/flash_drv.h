#ifndef FLASH_DRV_H
#define FLASH_DRV_H

#include <stdbool.h>
#include <stdint.h>

#include "macro_utils.h"

/*Hardware related code start*/
#define NOR_FLASH_BASE 0x08000000
#define NOR_FLASH_SIZE (1024 * K_BYTES)
#define NOR_FLASH_END (NOR_FLASH_BASE + NOR_FLASH_SIZE)
/*Hardware related code end*/
#define FLASH_WR_TIME_MS 20
#define FLASH_SECTOR_SIZE (128*K_BYTES)

bool flash_init(void);
bool flash_wr(uint32_t addr, uint8_t* array, uint32_t array_len);
bool flash_read(uint32_t in_flash_addr, uint8_t* rx_array, uint32_t array_len);
bool is_errased(uint32_t addr, uint32_t size);
bool is_flash_spare(uint32_t flash_addr, uint32_t size);
bool is_flash_addr(uint32_t flash_addr);
bool flash_find_spare_region(uint32_t* out_addr, uint32_t size);
bool flash_scan(uint8_t* base, uint32_t size, float* usage_pec, uint32_t* spare, uint32_t* busy);

#endif /* FLASH_DRV_H  */
