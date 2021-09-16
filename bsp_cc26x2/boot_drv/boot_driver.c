#include "boot_driver.h"

#include <stdbool.h>
#include <stdint.h>

#include "flash_drv.h"
#include "log.h"
#include "sys.h"

typedef void (*pFunction)(void);
pFunction Jump_To_Application;

bool jump_to_code(uint32_t app_start_address) {
    bool res = false;
    res = is_flash_addr(app_start_address);

    if(res) {
        uint32_t jump_address = 0;
        uint32_t stack_top = *(volatile uint32_t*)(app_start_address);
        if((RAM_START < stack_top) && (stack_top <= (RAM_START + RAM_SIZE))) {
            LOG_INFO(BOOT, "Jump to address 0x%08x", app_start_address);
            //  __disable_interrupt();
            jump_address = *((volatile uint32_t*)(app_start_address + 4));
            Jump_To_Application = (pFunction)jump_address;
            /* Initialize user application's Stack Pointer */
            __set_MSP(stack_top);
            Jump_To_Application();
            res = true;
        } else {
            res = false;
            LOG_ERROR(BOOT, "Error top stack size pointer 0x%08x lim: [0x%08x 0x%08x]", stack_top,RAM_START, RAM_START + RAM_SIZE);
        }
    }else{
        LOG_ERROR(BOOT, "Not a Flash 0x%08x", app_start_address);
    }
    return res;
}
