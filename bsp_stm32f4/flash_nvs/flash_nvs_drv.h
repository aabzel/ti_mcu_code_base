#ifndef FLASH_NVS_DRV_H
#define FLASH_NVS_DRV_H

#include <stdbool.h>
#include <stdint.h>

#define CONFIG_NVSINTERNAL 0
#define CONFIG_TI_DRIVERS_NVS_COUNT 1
#define CONFIG_NVS_COUNT 1
#define NVS_BUFF_SIZE 64

#define FLASH_WR_TIME_MS 20


bool flash_nvs_init(void);
bool flash_nvs_erase(uint32_t addr, uint32_t array_len);
bool flash_nvs_write(uint32_t addr, uint8_t* array, uint32_t array_len);
#endif /* FLASH_NVS_DRV_H  */
