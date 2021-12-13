#ifndef CORE_DRIVER_H
#define CORE_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <Temperature.h>
#include <TemperatureCC26X2.h>
#include <stdbool.h>
#include <stdint.h>

#define EXPECT_STACK_SIZE (4096 * 10)
#define CPU_SCS_BASE 0xE000E000
#define CPU_SCS_CPUID (CPU_SCS_BASE + 0xD00)

extern const TemperatureCC26X2_Config TemperatureCC26X2_config;

uint32_t cpu_get_id(void);
bool reboot(void);
uint64_t get_ble_mac(void);

#ifdef __cplusplus
}
#endif

#endif /* CORE_DRIVER_H */
