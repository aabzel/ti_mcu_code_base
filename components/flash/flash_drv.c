#include "flash_drv.h"

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "none_blocking_pause.h"

bool flash_init(void) {
    bool res = false;
    return res;
}

bool flash_write(uint32_t* addr, uint8_t* array, uint16_t array_len) { return false; }

bool flash_read(uint32_t* addr, uint8_t* rx_array, uint16_t array_len) { return false; }

bool flash_scan(uint8_t* base, uint32_t size, float* usage_pec, uint32_t* spare, uint32_t* busy) {
    bool res = false;
    if(usage_pec && spare && busy) {
        res = true;
        uint8_t* addr = base;
        uint32_t cnt = 0;
        for(addr = base, cnt = 0; addr < (base + size); addr++, cnt++) {
            if(0xFF == (*addr)) {
                (*spare)++;
            } else {
                (*busy)++;
            }
            if(!(cnt % 1000)) {
                wait_in_loop_ms(5);
            }
        }
        *usage_pec = (float)(((float)(100U * (*busy))) / ((float)size));
    }
    return res;
}
