#include "device_id.h"

#include <inttypes.h>
#include <stdint.h>

#include "core_driver.h"
#include "io_utils.h"
#include "read_mem.h"
#include "str_utils.h"
#include "version.h"

uint64_t get_device_serial(void) {
    uint32_t value;
#ifdef CC26XX
    value = read_addr_32bit(CPU_SCS_CPUID);
#endif
    return (uint64_t)value;
}
