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
Sector 0  0x08000000 - 0x08003FFF 0x04000 16  Kbytes
Sector 1  0x08004000 - 0x08007FFF 0x04000 16  Kbytes
Sector 2  0x08008000 - 0x0800BFFF 0x04000 16  Kbytes
Sector 3  0x0800C000 - 0x0800FFFF 0x04000 16  Kbyte
Sector 4  0x08010000 - 0x0801FFFF 0x10000 64  Kbytes
Sector 5  0x08020000 - 0x0803FFFF 0x20000 128 Kbytes
Sector 6  0x08040000 - 0x0805FFFF 0x20000 128 Kbytes
Sector 7  0x08060000 - 0x0807ffff 0x20000 128 Kbytes
Sector 8  0x08080000 - 0x0809ffff 0x20000 128 Kbytes
Sector 9  0x080a0000 - 0x080bffff 0x20000 128 Kbytes
Sector 10 0x080c0000 - 0x080dffff 0x20000 128 Kbytes
Sector 11 0x080E0000 - 0x080FFFFF 0x20000 128 Kbytes
*/
static bool Addr2SectorSize(uint32_t addr, uint32_t *sector, uint32_t *sec_size){
    bool res = true;
    if((0x08000000<=addr)&&(addr<=0x08003FFF)){
        *sector=0;
        *sec_size=16*K_BYTES;
    }else if((0x08004000<=addr)&&(addr<=0x08007FFF)){
        *sector=1;
        *sec_size=16*K_BYTES;
    }else if((0x08008000<=addr)&&(addr<=0x0800BFFF)){
        *sector=2;
        *sec_size=16*K_BYTES;
    }else if((0x0800C000<=addr)&&(addr<=0x0800FFFF)){
        *sector=3;
        *sec_size=16*K_BYTES;
    }else if((0x08010000<=addr)&&(addr<=0x0801FFFF)){
        *sector=4;
        *sec_size=64*K_BYTES;
    }else if((0x08020000<=addr)&&(addr<=0x0803FFFF)){
        *sector=5;
        *sec_size=128*K_BYTES;
    }else if((0x08040000<=addr)&&(addr<=0x0805FFFF)){
        *sector=6;
        *sec_size=128*K_BYTES;
    }else if((0x08060000<=addr)&&(addr<=0x0807ffff)){
        *sector=7;
        *sec_size=128*K_BYTES;
    }else if((0x08080000<=addr)&&(addr<=0x0809ffff)){
        *sector=8;
        *sec_size=128*K_BYTES;
    }else if((0x080a0000<=addr)&&(addr<=0x080bffff)){
        *sector=9;
        *sec_size=128*K_BYTES;
    }else if((0x080c0000<=addr)&&(addr<=0x080dffff)){
        *sector=10;
        *sec_size=128*K_BYTES;
    }else if((0x080E0000<=addr)&&(addr<=0x080FFFFF)){
        *sector=11;
        *sec_size=128*K_BYTES;
    }else{
        res = false;
        *sector=12;
        *sec_size=0;
    }
    return res;
}

bool flash_errase(uint32_t addr, uint32_t size){
    bool res = false;
    uint32_t sector = 0;
    uint32_t sec_cnt = 0;
    uint32_t sec_size = 0;
    res = is_errased(addr, size) ;
    if(false==res) {
        res=Addr2SectorSize(addr, &sector, &sec_size);
        if(res){
            if(size<=sec_size){
                sec_cnt=1;
            }else{
                sec_cnt=2;
            }
        }

        if(res){
            FLASH_EraseInitTypeDef EraseInitStruct;
            uint32_t SectorError = 0;
            EraseInitStruct.Banks = FLASH_BANK_1;
            EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
            EraseInitStruct.Sector = sector;
            EraseInitStruct.NbSectors = sec_cnt;
            EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;

            /* actual erase flash */
            HAL_FLASH_Unlock();
            HAL_StatusTypeDef ret = HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError) ;
            HAL_FLASH_Lock();
            if ( HAL_OK !=ret) {
                res =  false;
            }
        }
    }
    return res;
}
