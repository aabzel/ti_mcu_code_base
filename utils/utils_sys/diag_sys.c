#include "diag_sys.h"

#include <stdbool.h>
#include <stdint.h>

#include "debug_info.h"
#include "io_utils.h"
#include "sys_config.h"
#include "version.h"
#include "writer_generic.h"

#ifdef STM32F413xx
#include "cmsis_os.h"
#include "stm32f4xx_hal.h"
#endif /*STM32F413xx*/
#ifdef NORTOS
extern void main(void);
#endif

void print_sysinfo(void) {
    io_printf("Reset handler: 0x%x " CRLF, *((uint32_t*)0x00000004));
    // io_printf("addr of SystemInit() 0x%p" CRLF, SystemInit);
#ifdef NORTOS
    io_printf("addr of main() 0x%p" CRLF, main);
#endif
    io_printf("RAM: %u Byte" CRLF, RAM_SIZE);
    io_printf("Flash: %u Byte" CRLF, ROM_SIZE);
    io_printf("RAM addr:   0x%08x....0x%08x " CRLF, RAM_START, RAM_END);
    io_printf("Flash addr: 0x%08x....0x%08x " CRLF, ROM_START, ROM_END);
    io_printf("top-of-stack: %x " CRLF, *((uint32_t*)0x00000000));
    io_printf("boot memory start: %x " CRLF, *((uint32_t*)0x00000004));
}

void Error_Handler(void) {
#ifdef STM32F413xx
    __disable_irq();
#endif /*STM32F413xx*/
    while(1) {
    }
}
