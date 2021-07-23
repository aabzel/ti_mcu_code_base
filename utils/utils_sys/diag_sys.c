#include "diag_sys.h"

#include <stdbool.h>
#include <stdint.h>

#include "debug_info.h"
#include "io_utils.h"
#include "version.h"
#include "writer_generic.h"

bool find_addr_by_val(uint16_t byte_num, uint32_t val, uint32_t start_addr, uint32_t end_addr) {
    bool res = false;
    uint32_t cnt = 0;
    if(1 == byte_num) {
        uint8_t value = (uint8_t)val;
        uint8_t* ptr = (uint8_t*)start_addr;
        for(ptr = (uint8_t*)start_addr; ptr < ((uint8_t*)end_addr); ptr++) {
            if((*ptr) == value) {
                io_printf("%u: address *(0x%08p): 0x%04x/%u" CRLF, cnt, ptr, value, value);
                res = true;
            }
        }
    }

    if(2 == byte_num) {
        uint16_t value = (uint16_t)val;
        uint16_t* ptr = (uint16_t*)start_addr;
        for(ptr = (uint16_t*)start_addr; ptr < ((uint16_t*)end_addr); ptr++) {
            if((*ptr) == value) {
                io_printf("%u: addr *(0x%08p): 0x%04x/%u" CRLF, cnt, ptr, value, value);
                res = true;
            }
        }
    }

    if(4 == byte_num) {
        uint32_t value = (uint32_t)val;
        uint32_t* ptr = (uint32_t*)start_addr;
        for(ptr = (uint32_t*)start_addr; ptr < ((uint32_t*)end_addr); ptr++) {
            if((*ptr) == value) {
                io_printf("%u: addr *(0x%08p): 0x%08x/%u" CRLF, cnt, ptr, value, value);
                res = true;
            }
        }
    }
    return res;
}

extern void main(void);

void print_sysinfo(void) {
    io_printf("Reset handler: 0x%x " CRLF, *((uint32_t*)0x00000004));
    // io_printf("addr of SystemInit() 0x%p" CRLF, SystemInit);
    io_printf("addr of main() 0x%p" CRLF, main);

    io_printf("RAM: %u Byte" CRLF, RAM_SIZE);
    io_printf("Flash: %u Byte" CRLF, ROM_SIZE);
    io_printf("RAM addr:   0x%08x....0x%08x " CRLF, RAM_START, RAM_END);
    io_printf("Flash addr: 0x%08x....0x%08x " CRLF, ROM_START, ROM_END);
    io_printf("top-of-stack: %x " CRLF, *((uint32_t*)0x00000000));
    io_printf("boot memory start: %x " CRLF, *((uint32_t*)0x00000004));
}

void Error_Handler(void) {
    // __disable_irq();
    while(1) {
    }
}

bool print_mem(uint8_t* addr, int32_t len, bool new_line) {
    io_printf("0x");
    bool res = false;
    if(0 < len) {
        uint32_t pos;
        for(pos = 0; pos < len; pos++) {
            if(0 == (pos % 16)) {
                // io_printf(CRLF);
            }
            io_printf("%02x", *(addr + pos));

            res = true;
        }
        if(true == new_line) {
            io_printf(CRLF);
        }
    }
    return res;
}
