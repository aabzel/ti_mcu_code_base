#include "core_driver.h"

#include <string.h>
#include <sys_ctrl.h>
#include <hw_fcfg1.h>
#include <hw_memmap.h>
#include <Temperature.h>
#include <TemperatureCC26X2.h>

#include "bit_utils.h"
#include "data_utils.h"
#include "read_mem.h"
#ifdef HAS_WDT
#include "watchdog_drv.h"
#endif

const TemperatureCC26X2_Config TemperatureCC26X2_config = {
  //  .intPriority = 0xE0,
      .intPriority = (7<<5),
};

uint32_t cpu_get_id(void){
    uint32_t value;
    value = read_addr_32bit(CPU_SCS_CPUID);

    return value;
}

uint64_t get_ble_mac(void){
    Type64Union_t uValue ;
    uValue.u64=0;
    uValue.u32[0]=read_addr_32bit(FCFG1_BASE + FCFG1_O_MAC_BLE_0);
    uValue.u32[1]=read_addr_32bit(FCFG1_BASE + FCFG1_O_MAC_BLE_1);
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
