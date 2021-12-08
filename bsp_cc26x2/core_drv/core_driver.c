#include "core_driver.h"

#include <Temperature.h>
#include <TemperatureCC26X2.h>
#include <hw_fcfg1.h>
#include <hw_memmap.h>
#include <string.h>
#include <sys_ctrl.h>

#include "bit_utils.h"
#include "boot_cfg.h"
#include "data_utils.h"
#include "read_mem.h"
#include "sys.h"
#ifdef HAS_WDT
#include "watchdog_drv.h"
#endif

const TemperatureCC26X2_Config TemperatureCC26X2_config = {
    //  .intPriority = 0xE0,
    .intPriority = (7 << 5),
};

uint32_t cpu_get_id(void) {
    uint32_t value;
    value = read_addr_32bit(CPU_SCS_CPUID);

    return value;
}

uint64_t get_ble_mac(void) {
    Type64Union_t uValue;
    uValue.u64 = 0;
    uValue.u32[0] = read_addr_32bit(FCFG1_BASE + FCFG1_O_MAC_BLE_0);
    uValue.u32[1] = read_addr_32bit(FCFG1_BASE + FCFG1_O_MAC_BLE_1);
    uValue.u64 = MASK_48BIT & uValue.u64;
    return uValue.u64;
}

bool reboot(void) {
    bool res = false;
    SysCtrlSystemReset();
#ifdef HAS_WDT
    res = watchdog_set(10, 0);
#endif
    return res;
}

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
