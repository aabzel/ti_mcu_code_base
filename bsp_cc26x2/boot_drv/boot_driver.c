#include "boot_driver.h"

#include <stdbool.h>
#include <stdint.h>

#include "core_driver.h"
#include "flash_drv.h"
#include "log.h"
#include "read_mem.h"
#include "sys.h"

typedef void (*pFunction)(void);
pFunction Jump_To_Application;

bool boot_jump_to_code(uint32_t app_start_address) {
    bool res = false;
    res = is_flash_addr(app_start_address);

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


