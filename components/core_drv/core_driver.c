#include "core_driver.h"

#include <string.h>

#include "bit_utils.h"
#include "read_mem.h"

uint32_t cpu_get_id(void){
    uint32_t value;
    value = read_addr_32bit(CPU_SCS_CPUID);

    return value;
}


