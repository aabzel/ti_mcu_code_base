#ifndef BOOT_DRIVER_H
#define BOOT_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#define DFLT_APP_START_ADDR 0x0001A000
#define APP_LAYNCH_TRY 5U
#define FINE_START_TIME_OUT_MS 3000U
#define BOOT_PERIOD_US 2000000

typedef enum eCmdBoot_t {
    BOOT_CMD_LAUNCH_APP = 0,
    BOOT_CMD_LAUNCH_APP_CRC = 1,
    BOOT_CMD_STAY_ON = 2,
    BOOT_CMD_ENDEF = 3
} CmdBoot_t;

const char* boot_cmd2str(uint8_t boot_cmd);
bool boot_init(void);

#ifdef HAS_GENERIC
bool boot_jump_to_boot(void);
#endif

#ifdef HAS_BOOTLOADER
bool boot_try_app(void);
bool boot_erase_app(void);
bool boot_jump_to_code(uint32_t flash_addr);
bool boot_launch_app(void);
#endif

#ifdef HAS_GENERIC
bool boot_proc(void);
#endif

#ifdef __cplusplus
}
#endif

#endif /* BOOT_DRIVER_H */
