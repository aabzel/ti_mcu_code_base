
#include "ext_ram_sim_drv.h"

#include <stdbool.h>
#include <string.h>

uint32_t sem_err_cnt = 0;
SemaphoreHandle_t ext_ram_sem;
static volatile bool mem_busy = false;
uint8_t memory[EXT_RAM_SIZE];
uint32_t volatile ext_ram_busy_cnt = 0;
uint32_t sem_wait = 100;

bool ext_ram_read(uint32_t address, uint8_t* read_data, uint32_t size) {
    bool res = false;
    if(false == mem_busy) {
        mem_busy = true;
        if((address + size) <= sizeof(memory)) {
            memcpy(read_data, &memory[address], size);
            res = true;
        }
        mem_busy = false;
    } else {
        ext_ram_busy_cnt++;
    }
    return res;
}

bool ext_ram_write(uint32_t address, uint8_t* wr_data, uint32_t size) {
    bool res = false;
    if(false == mem_busy) {
        mem_busy = true;
        if((address + size) < sizeof(memory)) {
            memcpy(&memory[address], wr_data, size);
            res = true;
        }
        mem_busy = false;
    } else {
        ext_ram_busy_cnt++;
    }
    return res;
}

bool ext_ram_erase(void) {
    bool res = false;
    if(false == mem_busy) {
        mem_busy = true;
        memset(memory, 0xFF, sizeof(memory));
        mem_busy = false;
        res = true;
    } else {
        ext_ram_busy_cnt++;
    }
    return res;
}

bool ext_ram_init(void) {
    bool res = false;
    mem_busy = false;
    sem_wait = 100;
    ext_ram_sem = xSemaphoreCreateBinary();
    if(NULL == ext_ram_sem) {
        res = false;
    }else{
        xSemaphoreGive(ext_ram_sem);
    }
    
    return res;
}
