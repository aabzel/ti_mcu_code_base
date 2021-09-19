
#include "mx25l6433f_drv.h"

#include <stdbool.h>
#include <string.h>

uint8_t flash_ram_memory[];

bool mx25_read(uint32_t address, uint8_t* read_data, uint32_t size){
}

bool mx25_erase_chip(void){
    memset(fram_memory,0xFF,sizeof(fram_memory));
}