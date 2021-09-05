#ifndef NOR_FLASH_API_H
#define NOR_FLASH_API_H

#include <stdbool.h>
#include <stdint.h>

bool mm_flash_write(uint32_t address_des, uint8_t* address_src, uint32_t len);
bool mm_flash_zero(uint32_t address, uint32_t len);

#endif /* NOR_FLASH_API_H */
