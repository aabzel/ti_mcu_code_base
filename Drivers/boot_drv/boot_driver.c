#include "boot_driver.h"

#ifdef CC26XX
#include <hw_nvic.h>
#include <hw_types.h>
#include "systick.h"
#endif
#include <stdbool.h>
#include <stdint.h>

#ifdef USE_HAL_DRIVER
#include "cmsis_iccarm.h"
#endif

#include "clocks.h"
#include "core_driver.h"
#include "core_utils.h"
#include "flash_drv.h"
#include "flash_fs.h"
#ifdef HAS_LOG
#include "log.h"
#endif
#include "param_ids.h"
#include "read_mem.h"
#include "sys.h"
#include "sys_config.h"

#ifdef HAS_GENERIC
static bool fine_start_event = false;
#endif
#ifdef HAS_BOOTLOADER
typedef void (*pFunction)(void);
pFunction Jump_To_Application;
#endif

#if defined(CC26XX) && defined(HAS_BOOTLOADER)
static bool disable_interrupt(void) {
    HWREG(NVIC_DIS0) = 0xffffffff;
    HWREG(NVIC_DIS1) = 0xffffffff;
    return true;
}
#endif /*CC26XX*/

#ifdef HAS_BOOTLOADER
bool boot_jump_to_code(uint32_t app_start_address) {
    bool res = false;
    res = is_flash_addr(app_start_address);
#ifdef HAS_LOG
    LOG_INFO(BOOT, "Try boot 0x%08x", app_start_address);
#endif
    if(res) {
        uint32_t stack_top = read_addr_32bit(app_start_address);
        res = is_ram_addr(stack_top);
        if(res) {
            uint32_t reset_handler = 0;
#ifdef HAS_LOG
            LOG_INFO(BOOT, "Add stack_top address 0x%08x", stack_top);
            LOG_INFO(BOOT, "Jump to address 0x%08x", app_start_address);
#endif
            reset_handler = read_addr_32bit(app_start_address + 4);
#ifdef HAS_LOG
            LOG_INFO(BOOT, "App reset handler address 0x%08x", reset_handler);
#endif
#ifdef CC26XX
            disable_interrupt();
            SysTickDisable();
#endif
            sw_pause_ms(500);
            Jump_To_Application = (pFunction)reset_handler;
            /* Initialize user application's Stack Pointer */
            __set_MSP(stack_top);
            sw_pause_ms(500);
            Jump_To_Application();
            while(1){

            }
            //res = true;
        } else {
            res = false;
#ifdef HAS_LOG
            LOG_ERROR(BOOT, "Error top stack size pointer 0x%08x lim: [0x%08x 0x%08x]", stack_top, RAM_START,
                      RAM_START + RAM_SIZE);
#endif
#ifdef HAS_FLASH_FS
            uint8_t boot_cmd = BOOT_CMD_STAY_ON;
            res = mm_set(PAR_ID_BOOT_CMD, (uint8_t*)&boot_cmd, sizeof(boot_cmd));
            if(false == res) {
#ifdef HAS_LOG
                LOG_ERROR(BOOT, "Unable to send stay on");
#endif
            } else {
#ifdef HAS_LOG
                LOG_WARNING(BOOT, "Stay in boot");
#endif
            }
#endif /*HAS_FLASH_FS*/
        }
    } else {
#ifdef HAS_LOG
        LOG_ERROR(BOOT, "Not a Flash 0x%08x", app_start_address);
#endif
    }
    return res;
}
#endif

const char* boot_cmd2str(uint8_t boot_cmd) {
    const char* name = "error";
    switch(boot_cmd) {
    case BOOT_CMD_STAY_ON:
        name = "stay";
        break;
    case BOOT_CMD_LAUNCH_APP:
        name = "launch";
        break;
    case BOOT_CMD_LAUNCH_APP_CRC:
        name = "launchCrc";
        break;
    default:
        name = "undef";
        break;
    }
    return name;
}

#ifdef HAS_BOOTLOADER
bool boot_try_app(void) {
    bool res = false;
    uint16_t real_len = 0;
    CmdBoot_t boot_cmd = BOOT_CMD_ENDEF;
#ifdef HAS_LOG
    LOG_INFO(BOOT, "Try boot app...");
#endif
    res = mm_get(PAR_ID_BOOT_CMD, (uint8_t*)&boot_cmd, sizeof(boot_cmd), &real_len);
    if(res) {
        if(sizeof(boot_cmd) != real_len) {
            res = false;
#ifdef HAS_LOG
            LOG_ERROR(BOOT, "boot cmd len error %u", real_len);
#endif
        }
    } else {
#ifdef HAS_LOG
        LOG_ERROR(BOOT, "Lack of boot cmd ParamId: %u", PAR_ID_BOOT_CMD);
#endif
    }
#ifdef HAS_LOG
    LOG_INFO(BOOT, "Boot cmd %s", boot_cmd2str(boot_cmd));
#endif
    if(BOOT_CMD_STAY_ON == boot_cmd) {
        res = true;
    } else if(BOOT_CMD_LAUNCH_APP == boot_cmd) {
        uint32_t app_start_address = 0;
        res = mm_get(PAR_ID_APP_START, (uint8_t*)&app_start_address, sizeof(app_start_address), &real_len);
        if(res) {
            if(real_len == sizeof(app_start_address)) {
                res = boot_jump_to_code(app_start_address);
            } else {
#ifdef HAS_LOG
                LOG_ERROR(BOOT, "boot app address len error %u", real_len);
#endif
                res = false;
            }
        } else {
#ifdef HAS_LOG
            LOG_ERROR(BOOT, "Lack of boot app address");
#endif
            app_start_address = DFLT_APP_START_ADDR;
#ifdef HAS_FLASH_FS
            res = mm_set(PAR_ID_APP_START, (uint8_t*)&app_start_address, sizeof(app_start_address));
            if(false == res) {
#ifdef HAS_LOG
                LOG_ERROR(BOOT, "Error set dflt start adddr");
#endif
            }
#endif /*HAS_FLASH_FS*/
        }
    } else {
        res = false;
    }
    return res;
}
#endif

#ifdef HAS_BOOTLOADER
bool boot_launch_app(void) {
    bool res = false;
#ifdef HAS_FLASH_FS
    uint8_t boot_cmd = BOOT_CMD_LAUNCH_APP;
    res = mm_set(PAR_ID_BOOT_CMD, (uint8_t*)&boot_cmd, sizeof(boot_cmd));
    if(false == res) {
#ifdef HAS_LOG
        LOG_ERROR(BOOT, "Error set boot cmd");
#endif
    }
#endif /*HAS_FLASH_FS*/
#ifdef HAS_FLASH_FS
    uint8_t boot_cnt = 0;
    res = mm_set(PAR_ID_BOOT_CNT, (uint8_t*)&boot_cnt, sizeof(boot_cnt));
    if(false == res) {
#ifdef HAS_LOG
        LOG_ERROR(BOOT, "ErrorResetBootCnt");
#endif
    }
#endif /*HAS_FLASH_FS*/

#ifdef HAS_FLASH_FS
    uint32_t start_addr = DFLT_APP_START_ADDR;
    res = mm_set(PAR_ID_APP_START, (uint8_t*)&start_addr, sizeof(start_addr));
    if(false == res) {
#ifdef HAS_LOG
        LOG_ERROR(BOOT, "ErrorSetStartAddr");
#endif
    }
#endif /*HAS_FLASH_FS*/

    res = reboot();
    if(false == res) {
#ifdef HAS_LOG
        LOG_ERROR(BOOT, "ErrorReboot");
#endif
    }
    return res;
}
#endif

#ifdef HAS_GENERIC
bool boot_jump_to_boot(void) {
    bool res = false;
#ifdef HAS_FLASH_FS
    uint8_t boot_cmd = BOOT_CMD_STAY_ON;
    res = mm_set(PAR_ID_BOOT_CMD, (uint8_t*)&boot_cmd, sizeof(boot_cmd));
    if(false == res) {
#ifdef HAS_LOG
        LOG_ERROR(BOOT, "Error set boot cmd");
#endif
    }
#endif /*HAS_FLASH_FS*/

    res = reboot();
    if(false == res) {
#ifdef HAS_LOG
        LOG_ERROR(BOOT, "Error reboot");
#endif
    }
    return res;
}
#endif

/*Application Hang on protection*/
bool boot_init(void) {
    uint16_t real_len = 0;
    bool res = false;
    uint8_t boot_cnt = 0;
#ifdef HAS_GENERIC
    fine_start_event = false;
#endif
    res = mm_get(PAR_ID_BOOT_CNT, (uint8_t*)&boot_cnt, sizeof(boot_cnt), &real_len);
    if((true == res) && (sizeof(boot_cnt) == real_len)) {
#ifdef HAS_LOG
        LOG_INFO(BOOT, "launch try %u", boot_cnt);
#endif
        if(APP_LAYNCH_TRY < boot_cnt) {
#ifdef HAS_LOG
            LOG_ERROR(BOOT, "Application seems hang on");
#endif
#ifdef HAS_FLASH_FS
            CmdBoot_t boot_cmd = BOOT_CMD_STAY_ON;
            res = mm_set(PAR_ID_BOOT_CMD, (uint8_t*)&boot_cmd, sizeof(boot_cmd));
            if(false == res) {
#ifdef HAS_LOG
                LOG_ERROR(BOOT, "Unable to send boot cmd");
#endif
            } else {
#ifdef HAS_LOG
                LOG_DEBUG(BOOT, "Send boot stay on OK");
#endif
                res = true;
            }
#endif /*HAS_FLASH_FS*/
        } else {
            boot_cnt++;
#ifdef HAS_FLASH_FS
            res = mm_set(PAR_ID_BOOT_CNT, (uint8_t*)&boot_cnt, sizeof(boot_cnt));
            if(false == res) {
#ifdef HAS_LOG
                LOG_ERROR(BOOT, "Unable to update boot cnt");
#endif
            }
#endif /*HAS_FLASH_FS*/
        }
    } else {
        res = true;
        boot_cnt = 0;
#ifdef HAS_FLASH_FS
        res = mm_set(PAR_ID_BOOT_CNT, (uint8_t*)&boot_cnt, sizeof(boot_cnt));
        if(false == res) {
#ifdef HAS_LOG
            LOG_ERROR(BOOT, "Unable to init boot cnt");
#endif
        }
#endif /*HAS_FLASH_FS*/
    }
    return res;
}
#ifdef HAS_GENERIC
bool boot_proc(void) {
    bool res = false;
    uint32_t up_time_ms = get_time_ms32();
    if((2 * WDT_TIMEOUT_MS) < up_time_ms) {
        /* Watchdog did not reset board still.
           That means that everything all right.*/
        if(false == fine_start_event) {
            /*Indicate boot that Application loaded fine*/
#ifdef HAS_FLASH_FS
            uint8_t boot_cnt = 0;
            res = mm_set(PAR_ID_BOOT_CNT, (uint8_t*)&boot_cnt, sizeof(boot_cnt));
            if(false == res) {
#ifdef HAS_LOG
                LOG_ERROR(BOOT, "Unable to reset boot cnt");
#endif
            } else {
                res = true;
#ifdef HAS_LOG
                LOG_INFO(BOOT, "AppLoadedFine!");
#endif
            }
#endif /*HAS_FLASH_FS*/
        }
        fine_start_event = true;
    }
    return res;
}
#endif
