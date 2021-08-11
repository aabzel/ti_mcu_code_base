#ifndef CORE_DRIVER_H
#define CORE_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#define CPU_SCS_BASE 0xE000E000
#define CPU_SCS_CPUID (CPU_SCS_BASE+0xD00)

uint32_t cpu_get_id(void);
bool reboot(void);

#ifdef __cplusplus
}
#endif

#endif /* CORE_DRIVER_H */
