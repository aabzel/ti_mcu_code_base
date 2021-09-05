/**
 * NorFlash Memory manager
 * This file contains implementation of key-val(file) map in Nor-Flash with 
 * uniform deterioration of Nor Flash memory
 *
 */
#include "flash_fs_ll.h"

#include <stdbool.h>
#include <string.h>

#include "flash_fs.h"
#include "flash_drv.h"
#include "memory_layout.h"


/**
 *     @brief    write specified number of bytes to the flash (4 bytes aligned only)
 *
 *     @param    addressDes - desitnation address, where data should be placed
 *     @param    addressSrc - source address that contain data should be writted
 *     @param    number of bytes that should be written (must be 4 bytes aligned)
 *     @retval    MM_RET code
 */
bool mm_flash_write(uint32_t address_des, uint8_t* address_src, uint32_t len) {
    bool res = true;
    res= flash_write(address_des,(uint8_t*) address_src, len);
    return res;
}

bool mm_flash_zero(uint32_t address, uint32_t len){
    bool res = false;
    uint8_t zeroArray[len];
    memset(zeroArray,00,len);
    res= flash_write(address,(uint8_t*) zeroArray, len);
    return res;
}

