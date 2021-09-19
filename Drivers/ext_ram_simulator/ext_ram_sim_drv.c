
#include "ext_ram_sim_drv.h"

#include <stdbool.h>
#include <string.h>

static bool mem_busy = false;
uint8_t memory[EXT_RAM_SIZE];

bool ext_ram_read(uint32_t address, uint8_t* read_data, uint32_t size) {
    bool res = false;
    if(false == mem_busy) {
        mem_busy = true;
        if((address + size) <= sizeof(memory)) {
            memcpy(read_data, &memory[address], size);
            res = true;
        }
        mem_busy = false;
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
    }
    return res;
}

bool ext_ram_erase(void) {
    bool res = false;
    if(false == mem_busy) {
        mem_busy = true;
        memset(memory, 0xFF, sizeof(memory));
        mem_busy = false;
    }
    return res;
}

bool ext_ram_init(void) {
    mem_busy = false;
    return true;
}
