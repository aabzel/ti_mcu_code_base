/*
 see page 79   1.3.2.2 Flash Memory
 see page 631 8.5 FLASH
 flash.h
 */
#ifndef FLASH_DRV_H
#define FLASH_DRV_H

#include <stdbool.h>
#include <stdint.h>


#include "macro_utils.h"

/*Hardware related code start*/
#define NOR_FLASH_BASE 0x00000000
#define NOR_FLASH_SIZE (352 * K_BYTES)
#define NOR_FLASH_END (NOR_FLASH_BASE + NOR_FLASH_SIZE)
#define FLASH_SECTOR_SIZE (8 * K_BYTES)
/*Hardware related code end*/

#define CONFIG_NVSINTERNAL 0
#define CONFIG_TI_DRIVERS_NVS_COUNT 1
#define CONFIG_NVS_COUNT 1
#define NVS_BUFF_SIZE 64

#define FLASH_WR_TIME_MS 20

bool flash_init(void);
bool flash_wr(uint32_t addr, uint8_t* array, uint32_t array_len);
bool flash_erase_pages(uint8_t page_start,uint8_t page_end );
bool flash_erase_sector(uint32_t sector_address);
bool flash_read(uint32_t in_flash_addr, uint8_t* rx_array, uint32_t array_len);
bool is_errased(uint32_t addr, uint32_t size);
bool is_addr_protected(uint32_t flash_addr);
bool is_flash_spare(uint32_t flash_addr, uint32_t size);
bool is_flash_addr(uint32_t flash_addr);
bool flash_find_spare_region(uint32_t* out_addr, uint32_t size);
uint32_t flash_page2addr(uint8_t page);
/*
 * API
 erase 8kbyte flash block
 programm 64bit (8 byte) qword
 is block read only
 * */

bool flash_scan(uint8_t* base, uint32_t size, float* usage_pec, uint32_t* spare, uint32_t* busy);

#endif /* FLASH_DRV_H  */
