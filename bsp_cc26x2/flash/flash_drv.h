/*
 see page 79   1.3.2.2 Flash Memory
 see page 631 8.5 FLASH
 flash.h
 */
#ifndef FLASH_DRV_H
#define FLASH_DRV_H

#include <stdbool.h>
#include <stdint.h>
#include <ti/drivers/NVS.h>

#include "flash_drv.h"

#define K_BYTES 1024
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

extern const uint_least8_t CONFIG_NVSINTERNAL_CONST;
extern uint8_t nvs_buffer[NVS_BUFF_SIZE];
extern NVS_Handle nvsHandle;
extern NVS_Attrs regionAttrs;

bool flash_init(void);
bool flash_write(uint32_t addr, uint8_t* array, uint32_t array_len);
bool flash_read(uint32_t* addr, uint8_t* rx_array, uint32_t array_len);
bool flash_erase(uint32_t addr, uint32_t array_len);
bool is_errased(uint32_t addr, uint32_t size);
bool is_addr_protected(uint32_t flash_addr);
bool is_flash_addr(uint32_t flash_addr);
/*
 * API
 erase 8kbyte flash block
 programm 64bit (8 byte) qword
 is block read only
 * */

bool flash_scan(uint8_t* base, uint32_t size, float* usage_pec, uint32_t* spare, uint32_t* busy);

#endif /* FLASH_DRV_H  */
