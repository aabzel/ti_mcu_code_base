#include "boot_driver.h"

#include <stdbool.h>
#include <stdint.h>

#include "core_driver.h"
#include "flash_drv.h"
#include "flash_fs.h"
#include "log.h"
#include "param_ids.h"
#include "read_mem.h"
#include "sys.h"

typedef void (*pFunction)(void);
pFunction Jump_To_Application;

bool boot_jump_to_code(uint32_t app_start_address) {
    bool res = false;
    res = is_flash_addr(app_start_address);
    LOG_INFO(BOOT, "Try boot 0x%08x", app_start_address);
    if(res) {
        uint32_t stack_top = read_addr_32bit(app_start_address);
        res = is_ram_addr(stack_top);
        if(res) {
            uint32_t reset_handler = 0;
            LOG_INFO(BOOT, "Jump to address 0x%08x", app_start_address);
            //  __disable_interrupt();
            reset_handler = read_addr_32bit(app_start_address + 4);
            Jump_To_Application = (pFunction)reset_handler;
            /* Initialize user application's Stack Pointer */
            __set_MSP(stack_top);
            Jump_To_Application();
            res = true;
        } else {
            res = false;
            LOG_ERROR(BOOT, "Error top stack size pointer 0x%08x lim: [0x%08x 0x%08x]", stack_top, RAM_START,
                      RAM_START + RAM_SIZE);
        }
    } else {
        LOG_ERROR(BOOT, "Not a Flash 0x%08x", app_start_address);
    }
    return res;
}

bool boot_try_app(void) {
    bool res = false;
    uint16_t real_len = 0;
    uint32_t app_start_address = 0;
    res = mm_get(PAR_ID_APP_START, (uint8_t*)&app_start_address, sizeof(app_start_address), &real_len);
    if(res) {
        if(real_len == sizeof(app_start_address)) {
            res = boot_jump_to_code(app_start_address);
        } else {
            LOG_ERROR(BOOT, "boot app address len error %u", real_len);
        }
    } else {
        LOG_ERROR(BOOT, "Lack of boot app address");
    }
    return res;
}

bool boot_erase_app(void) { return false; }
