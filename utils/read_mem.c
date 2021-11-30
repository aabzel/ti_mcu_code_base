#include "read_mem.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

uint8_t read_addr_8bit(uint32_t address) {
    uint8_t value = 0u;

    volatile uint8_t* p_addr = NULL;
    /*MISRA 2012 Rule 11.4: integer should not be converted to pointer */
    p_addr = (volatile uint8_t*)address;
    if(p_addr){
        value = *p_addr;
    }

    return value;
}

uint16_t read_addr_16bit(uint32_t address) {
    uint16_t value = 0u;

    volatile uint16_t* p_addr = NULL;
    /*MISRA 2012 Rule 11.4: integer should not be converted to pointer */
    p_addr = (volatile uint16_t*)address;
    value = *p_addr;

    return value;
}

uint32_t read_addr_32bit(uint32_t address) {
    uint32_t value = 0u;
    volatile uint32_t* addr = NULL;
    /*MISRA 2012 Rule 11.4: integer should not be converted to pointer */
    addr = (volatile uint32_t*)address;
    value = *addr;

    return value;
}

/*
 Works with RAM REG
 Will not work with ROM Flath
 * */
bool write_addr_32bit(uint32_t address, uint32_t value) {
    bool res = false;
    res = true;
    volatile uint32_t* addr = NULL;
    addr = (volatile uint32_t*)address;
    (*addr) = value;

    return res;
}

uint64_t read_addr_64bit(uint32_t address) {
    uint64_t value = 0u;
    volatile uint64_t* addr = NULL;
    /*MISRA 2012 Rule 11.4: integer should not be converted to pointer */
    addr = (volatile uint64_t*)address;
    value = *addr;

    return value;
}
