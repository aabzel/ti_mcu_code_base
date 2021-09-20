#ifndef BOOT_DRIVER_H
#define BOOT_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

typedef enum eCmdBoot_t{
    BOOT_CMD_LAUNCH_APP =0,
    BOOT_CMD_STAY_ON =1,
    BOOT_CMD_ENDEF=2
}CmdBoot_t;

bool boot_erase_app(void);
bool boot_jump_to_code(uint32_t flash_addr);
bool boot_try_app(void);

#ifdef __cplusplus
}
#endif

#endif /* BOOT_DRIVER_H */
