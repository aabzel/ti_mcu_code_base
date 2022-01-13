#include "core_utils.h"

#include <string.h>

#include "array.h"
#include "bit_utils.h"
#ifdef HAS_BOOT
#include "boot_cfg.h"
#endif
#include "core_driver.h"
#include "data_utils.h"
#include "read_mem.h"
#include "sys.h"
#include "sys_config.h"

bool is_ram_addr(uint32_t address) {
    bool res = false;
    if((RAM_START < address) && (address <= (RAM_START + RAM_SIZE))) {
        res = true;
    }
    return res;
}

static bool call_recursion(uint32_t cur_depth, uint32_t max_depth, uint32_t* stack_size) {
    bool res = false;
    if(cur_depth < max_depth) {
        res = call_recursion(cur_depth + 1, max_depth, stack_size);
    } else if(cur_depth == max_depth) {
        uint32_t top_stack_val = *((uint32_t*)(APP_START_ADDRESS));
        uint32_t cur_stack_use = top_stack_val - ((uint32_t)&res);
        *stack_size = cur_stack_use;
        res = true;
    } else {
        res = false;
    }
    return res;
}

bool try_recursion(uint32_t max_depth, uint32_t* stack_size) {
    bool res = false;
    res = call_recursion(0, max_depth, stack_size);
    return res;
}

float stack_used(void) {
    float precent=0.0f;
    uint32_t busy = 0;
    uint32_t top_stack_val = *((uint32_t*)(APP_START_ADDRESS));
    uint32_t max_cont_patt = 0;
    bool res = array_max_cont((uint8_t*)top_stack_val - EXPECT_STACK_SIZE, EXPECT_STACK_SIZE, 0, &max_cont_patt);
    busy = EXPECT_STACK_SIZE - max_cont_patt;
    if(res) {
        precent = ((float)100 * busy) / ((float)EXPECT_STACK_SIZE);
    }

    return precent;
}


