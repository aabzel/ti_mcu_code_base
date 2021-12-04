#include "flash_nvs_drv.h"


#include <stdint.h>
#include <stddef.h>

#include "bit_utils.h"
#include "data_utils.h"
#include "memory_layout.h"

 
bool flash_nvs_init(void) {
    bool res = false;
    return res;
}

bool flash_nvs_write(uint32_t flas_addr, uint8_t* array, uint32_t array_len) {
    bool res = false;
    if((NVS_FLASH_START <= flas_addr) && (flas_addr < (NVS_FLASH_START + NVS_SIZE))) {
        size_t offset = flas_addr - NVS_FLASH_START;
        (void)offset;
    }
    return res;
}

bool flash_nvs_erase(uint32_t addr, uint32_t array_len) {
    bool res = false;
    if((NVS_FLASH_START <= addr) && addr < (NVS_FLASH_START + NVS_SIZE)) {
    }
    return res;
}


