#ifndef BOOT_DRIVER_H
#define BOOT_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#define APP_LAYNCH_TRY 5U

typedef enum eCmdBoot_t {
    BOOT_CMD_LAUNCH_APP = 0,
    BOOT_CMD_LAUNCH_APP_CRC = 1,
    BOOT_CMD_STAY_ON = 2,
    BOOT_CMD_ENDEF = 3 }
CmdBoot_t;

bool boot_init(void);
bool boot_erase_app(void);
bool boot_jump_to_code(uint32_t flash_addr);
bool boot_try_app(void);
bool boot_proc(void);

#ifdef __cplusplus
}
#endif

#endif /* BOOT_DRIVER_H */
