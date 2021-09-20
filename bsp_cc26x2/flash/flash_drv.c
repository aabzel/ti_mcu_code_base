#include "flash_drv.h"

#include <flash.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <hw_ccfg.h>
#include <hw_memmap.h>
#include <hw_types.h>
#include <vims.h>

#include "bit_utils.h"
#include "data_utils.h"
#include "memory_layout.h"
#include "none_blocking_pause.h"




bool flash_init(void) {
    bool res = true;
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

bool flash_scan(uint8_t* base, uint32_t size, float* usage_pec, uint32_t* spare, uint32_t* busy) {
    bool res = false;
    if(usage_pec && spare && busy) {
        res = true;
        *spare = 0;
        *busy = 0;
        uint8_t* addr = base;
        uint32_t cnt = 0;
        for(addr = base, cnt = 0; addr < (base + size); addr++, cnt++) {
            res = is_flash_addr((uint32_t)addr);
            if(false == res) {
                break;
            }
            if(0xFF == (*addr)) {
                (*spare)++;
            } else {
                (*busy)++;
            }
            if(!(cnt % 1000)) {
                // wait_in_loop_ms(5);
            }
        }
        *usage_pec = (float)(((float)(100U * (*busy))) / ((float)size));
    }
    return res;
}

bool flash_wr(uint32_t flash_addr, uint8_t* wr_array, uint32_t array_len) {
    bool res = false, loop = true;
    uint32_t ret = 0, cnt = 0;
    VIMSModeSet(VIMS_BASE, VIMS_MODE_OFF);
    VIMSLineBufDisable(VIMS_BASE);
    ret = FlashProgram(wr_array, flash_addr, array_len);
    if(FAPI_STATUS_SUCCESS == ret) {
        res = true;
        uint8_t readMem[array_len];
        while(loop) {
            flash_read(flash_addr, readMem, sizeof(readMem));
            ret = memcmp(readMem, wr_array, array_len);
            if(0 == ret) {
                loop = false;
                res = true;
            }
            wait_ms(FLASH_WR_TIME_MS);
            cnt++;
            if(1000 < cnt) {
                loop = false;
                res = false;
            }
        }
    }
    VIMSLineBufEnable(VIMS_BASE);
    VIMSModeSet(VIMS_BASE, VIMS_MODE_ENABLED);
    return res;
}

bool flash_erase_sector(uint32_t sector_address) {
    bool res = false;
    if((0 == (sector_address % FLASH_SECTOR_SIZE)) || (0 == sector_address)) {
        VIMSModeSet(VIMS_BASE, VIMS_MODE_OFF);
        VIMSLineBufDisable(VIMS_BASE);
        uint32_t ret = FlashSectorErase(sector_address);
        if(FAPI_STATUS_SUCCESS == ret) {
            res = true;
        }
        VIMSLineBufEnable(VIMS_BASE);
        VIMSModeSet(VIMS_BASE, VIMS_MODE_ENABLED);
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

bool is_addr_protected(uint32_t flash_addr) {
    bool res = false;
    if(is_flash_addr(flash_addr)) {
        res = true;
        uint32_t reg_val = 0;
        uint8_t flash_sector = flash_addr / FLASH_SECTOR_SIZE;
        if(flash_sector <= 31) {
            reg_val = HWREG(CCFG_BASE + CCFG_O_CCFG_PROT_31_0);
        } else if((32 <= flash_sector) && (flash_sector <= 63)) {
            reg_val = HWREG(CCFG_BASE + CCFG_O_CCFG_PROT_63_32);
            flash_sector -= 32;
        } else if((64 <= flash_sector) && (flash_sector <= 95)) {
            reg_val = HWREG(CCFG_BASE + CCFG_O_CCFG_PROT_95_64);
            flash_sector -= 64;
        } else if((96 <= flash_sector) && (flash_sector <= 127)) {
            reg_val = HWREG(CCFG_BASE + CCFG_O_CCFG_PROT_127_96);
            flash_sector -= 96;
        } else {
            res = false;
        }
        if(res) {
            res = IS_BIT_RESET(reg_val, flash_sector);
        }
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

uint32_t flash_page2addr(uint8_t page){
    uint32_t address = page*FLASH_SECTOR_SIZE;
    return address;
}

bool flash_erase_pages(uint8_t page_start,uint8_t page_end ) {
    bool res = false;
    if(page_start<=page_end){
        res = true;
        uint8_t page = 0;
        uint32_t sector_address=0;
        for(page=page_start; page<=page_end; page++) {
            sector_address = flash_page2addr(  page);
            res = flash_erase_sector(  sector_address)&&res;
        }
    }
    return res;
}

