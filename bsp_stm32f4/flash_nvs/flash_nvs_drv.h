#ifndef FLASH_NVS_DRV_H
#define FLASH_NVS_DRV_H

#include <stdbool.h>
#include <stdint.h>

#include "flash_drv.h"

#define MIN_SIZE_OF_ITEM (sizeof(mmItem_t)+QWORD_LEN)

bool flash_nvs_init(void);
bool flash_nvs_erase(uint32_t addr, uint32_t array_len);
bool flash_nvs_write(uint32_t addr, uint8_t* array, uint32_t array_len);
#endif /* FLASH_NVS_DRV_H  */
