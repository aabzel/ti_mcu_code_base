#ifndef FLASH_DRV_H
#define FLASH_DRV_H

#include <stdbool.h>
#include <stdint.h>

#define NOR_FLASH_BASE 0x00000000
#define NOR_FLASH_SIZE (352 * 1024)

bool flash_init(void);
bool flash_write(uint32_t* addr, uint8_t* array, uint16_t array_len);
bool flash_read(uint32_t* addr, uint8_t* rx_array, uint16_t array_len);

bool flash_scan(uint8_t* base, uint32_t size, float* usage_pec, uint32_t* spare, uint32_t* busy);

#endif /* FLASH_DRV_H  */
