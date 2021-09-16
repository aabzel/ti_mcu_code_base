#ifndef BOOT_DRIVER_H
#define BOOT_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

bool boot_jump_to_code(uint32_t flash_addr);
bool boot_try_app(void);

#ifdef __cplusplus
}
#endif

#endif /* BOOT_DRIVER_H */
