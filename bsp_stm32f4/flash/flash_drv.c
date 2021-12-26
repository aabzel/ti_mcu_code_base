#include "flash_drv.h"

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "array.h"
#include "bit_utils.h"
#include "data_utils.h"
#include "none_blocking_pause.h"
#include "stm32f4xx_hal.h"

bool flash_init(void) {
    bool res = false;
    return res;
}

bool flash_read(uint32_t in_flash_addr, uint8_t* rx_array, uint32_t array_len) {
    bool res = false;
    uint32_t i = 0;
    uint8_t* p_flash_addr = (uint8_t*)in_flash_addr;
    for(i = 0; i < array_len; i++) {
        res = is_flash_addr((uint32_t)p_flash_addr);
        if(res) {
            rx_array[i] = *(p_flash_addr);
            p_flash_addr++;
        } else {
            break;
        }
    }
    return res;
}

bool flash_wr(uint32_t flash_addr, uint32_t* wr_array, uint32_t len) {
    bool res = true;
    /* ensure that data is 4 bytes aligned */
    if ((len & 3) != 0) {
        res=false;
    }else{
        uint32_t i = 0, j = 0;
        /* write data to flash */
        HAL_FLASH_Unlock();
        for (i = 0, j = 0; j < len; i++, j += QWORD_LEN) {
            if (HAL_OK==HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, flash_addr, wr_array[i])) {
                flash_addr += QWORD_LEN;
            } else {
                HAL_FLASH_Lock();
                return false;
            }
        }
        HAL_FLASH_Lock();
    }
    return res;
}

bool is_errased(uint32_t addr, uint32_t size) {
    bool res = false;
    res = is_arr_pat((uint8_t*)addr, size, 0xff);
    return res;
}

bool is_flash_addr(uint32_t flash_addr) {
    bool res = false;
    if(flash_addr < NOR_FLASH_END) {
        res = true;
    }
    return res;
}

/*
 Is there a continuously free block of given size starting at the flash_addr
 */
bool is_flash_spare(uint32_t flash_addr, uint32_t size) {
    uint32_t spare_size = 0;
    uint32_t busy_size = 0;
    bool res = false;
    float usage_pec = 0.0f;
    res = flash_scan((uint8_t*)flash_addr, size, &usage_pec, &spare_size, &busy_size);
    if(size == spare_size) {
        res = true;
    } else {
        res = false;
    }

    return res;
}

/*
 * find the first address of spare continuous block of given size
 * returns the start address
 * */
bool flash_find_spare_region(uint32_t* out_addr, uint32_t size) {
    uint32_t flash_addr = NOR_FLASH_BASE;
    bool res = false;
    for(flash_addr = NOR_FLASH_BASE; flash_addr < NOR_FLASH_END; flash_addr++) {
        res = is_flash_spare(flash_addr, size);
        if(res) {
            *out_addr = flash_addr;
            break;
        }
    }
    return res;
}

/*
Sector 0 0x08000000 - 0x08003FFF 16 Kbytes
Sector 1 0x08004000 - 0x08007FFF 16 Kbytes
Sector 2 0x08008000 - 0x0800BFFF 16 Kbytes
Sector 3 0x0800C000 - 0x0800FFFF 16 Kbyte
Sector 4 0x08010000 - 0x0801FFFF 64 Kbytes
Sector 5 0x08020000 - 0x0803FFFF 128 Kbytes
Sector 6 0x08040000 - 0x0805FFFF 128 Kbytes
Sector 7 128 Kbytes
Sector 8 128 Kbytes
Sector 9 128 Kbytes
Sector 10 128 Kbytes
Sector 11 0x080E0000 - 0x080FFFFF 128 Kbytes
*/
