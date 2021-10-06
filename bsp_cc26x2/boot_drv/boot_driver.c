#include "boot_driver.h"

#include <hw_nvic.h>
#include <hw_types.h>
#include <stdbool.h>
#include <stdint.h>

#include "clocks.h"
#include "core_driver.h"
#include "flash_drv.h"
#include "flash_fs.h"
#include "log.h"
#include "param_ids.h"
#include "read_mem.h"
#include "sys.h"
#include "sys_config.h"

static bool fine_start_event = false;
typedef void (*pFunction)(void);
pFunction Jump_To_Application;

static bool disable_interrupt(void) {
    HWREG(NVIC_DIS0) = 0xffffffff;
    HWREG(NVIC_DIS1) = 0xffffffff;
    return true;
}

bool boot_jump_to_code(uint32_t app_start_address) {
    bool res = false;
    res = is_flash_addr(app_start_address);
    LOG_INFO(BOOT, "Try boot 0x%08x", app_start_address);
    if(res) {
        uint32_t stack_top = read_addr_32bit(app_start_address);
        res = is_ram_addr(stack_top);
        if(res) {
            uint32_t reset_handler = 0;
            LOG_INFO(BOOT, "stack_top address 0x%08x", stack_top);
            LOG_INFO(BOOT, "Jump to address 0x%08x", app_start_address);
            disable_interrupt();
            reset_handler = read_addr_32bit(app_start_address + 4);
            LOG_INFO(BOOT, "App reset handler address 0x%08x", reset_handler);
            Jump_To_Application = (pFunction)reset_handler;
            /* Initialize user application's Stack Pointer */
            __set_MSP(stack_top);
            Jump_To_Application();
            res = true;
        } else {
            res = false;
            LOG_ERROR(BOOT, "Error top stack size pointer 0x%08x lim: [0x%08x 0x%08x]", stack_top, RAM_START,
                      RAM_START + RAM_SIZE);
            uint8_t boot_cmd = BOOT_CMD_STAY_ON;
            res = mm_set(PAR_ID_BOOT_CMD, (uint8_t*)&boot_cmd, sizeof(boot_cmd));
            if(false == res) {
                LOG_ERROR(BOOT, "Unable to send stay on");
            } else {
                LOG_WARNING(BOOT, "Stay in boot");
            }
        }
    } else {
        LOG_ERROR(BOOT, "Not a Flash 0x%08x", app_start_address);
    }
    return res;
}

static const char* boot_cmd2str(CmdBoot_t boot_cmd) {
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

bool boot_try_app(void) {
    bool res = false;
    uint16_t real_len = 0;
    CmdBoot_t boot_cmd = BOOT_CMD_ENDEF;
    LOG_INFO(BOOT, "Try boot app...");
    res = mm_get(PAR_ID_BOOT_CMD, (uint8_t*)&boot_cmd, sizeof(boot_cmd), &real_len);
    if(res) {
        if(sizeof(boot_cmd) != real_len) {
            res = false;
            LOG_ERROR(BOOT, "boot cmd len error %u", real_len);
        }
    } else {
        LOG_ERROR(BOOT, "Lack of boot cmd ParamId: %u", PAR_ID_BOOT_CMD);
    }
    LOG_INFO(BOOT, "Boot cmd %s", boot_cmd2str(boot_cmd));
    if(BOOT_CMD_STAY_ON == boot_cmd) {
        res = true;
    } else if(BOOT_CMD_LAUNCH_APP == boot_cmd) {
        uint32_t app_start_address = 0;
        res = mm_get(PAR_ID_APP_START, (uint8_t*)&app_start_address, sizeof(app_start_address), &real_len);
        if(res) {
            if(real_len == sizeof(app_start_address)) {
                res = boot_jump_to_code(app_start_address);
            } else {
                LOG_ERROR(BOOT, "boot app address len error %u", real_len);
                res = false;
            }
        } else {
            LOG_ERROR(BOOT, "Lack of boot app address");
            app_start_address = DFLT_APP_START_ADDR;
            res = mm_set(PAR_ID_APP_START, (uint8_t*)&app_start_address, sizeof(app_start_address));
            if(false == res) {
                LOG_ERROR(BOOT, "Error set dflt start adddr");
            }
        }
    } else {
        res = false;
    }
    return res;
}

/*Application Hang on protection*/
bool boot_init(void) {
    uint16_t real_len = 0;
    bool res = false;
    uint8_t boot_cnt = 0;
    fine_start_event = false;
    res = mm_get(PAR_ID_BOOT_CNT, (uint8_t*)&boot_cnt, sizeof(boot_cnt), &real_len);
    if((true == res) && (sizeof(boot_cnt) == real_len)) {
        LOG_INFO(BOOT, "launch try %u", boot_cnt);
        if(APP_LAYNCH_TRY < boot_cnt) {
            LOG_ERROR(BOOT, "Application seems hang on");
            CmdBoot_t boot_cmd = BOOT_CMD_STAY_ON;
            res = mm_set(PAR_ID_BOOT_CMD, (uint8_t*)&boot_cmd, sizeof(boot_cmd));
            if(false == res) {
                LOG_ERROR(BOOT, "Unable to send boot cmd");
            } else {
                LOG_DEBUG(BOOT, "Send boot stay on OK");
                res = true;
            }
        } else {
            boot_cnt++;
            res = mm_set(PAR_ID_BOOT_CNT, (uint8_t*)&boot_cnt, sizeof(boot_cnt));
            if(false == res) {
                LOG_ERROR(BOOT, "Unable to update boot cnt");
            }
        }
    } else {
        res = true;
        boot_cnt = 0;
        res = mm_set(PAR_ID_BOOT_CNT, (uint8_t*)&boot_cnt, sizeof(boot_cnt));
        if(false == res) {
            LOG_ERROR(BOOT, "Unable to init boot cnt");
        }
    }
    return res;
}

bool boot_proc(void) {
    bool res = false;
    uint32_t up_time_ms = get_time_ms32();
    if((2 * WDT_TIMEOUT_MS) < up_time_ms) {
        if(false == fine_start_event) {
            /*Indicate boot that Application loaded fine*/
            uint8_t boot_cnt = 0;
            res = mm_set(PAR_ID_BOOT_CNT, (uint8_t*)&boot_cnt, sizeof(boot_cnt));
            if(false == res) {
                LOG_ERROR(BOOT, "Unable to reset boot cnt");
            } else {
                res = true;
                LOG_INFO(BOOT, "App loaded fine");
            }
        }
        fine_start_event = true;
    }
    return res;
}
