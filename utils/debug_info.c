#include "debug_info.h"

#include <inttypes.h>
#include <string.h>

#include "byte_utils.h"
#include "convert.h"
#include "device_id.h"
#include "io_utils.h"
#include "oprintf.h"
#include "sys.h"
#include "uart_drv.h"
#include "version.h"
#include "writer_generic.h"

static bool stack_dir(int32_t* main_local_addr) {
    bool res = false;
    int32_t fun_local;
    if(((void*)main_local_addr) < ((void*)&fun_local)) {
        io_printf("Stack grows from small addr to big addr -> \n" CRLF);
    } else {
        io_printf("Stack grows from big addr to small addr <- \n" CRLF);
    }
    return res;
}

bool explore_stack_dir(void) {
    // fun's local variable
    bool res = false;
    int32_t main_local;
    res = stack_dir(&main_local);
    return res;
}

bool is_big_endian(void) {
    U32_bit_t bint;
    bint.u32 = 0x01020304;
    return bint.u8[0] == 1;
}

bool is_little_endian(void) {
    U32_bit_t bint;
    bint.u32 = 0x01020304;
    return bint.u8[0] == 4;
}

void print_version_s(ostream_t* stream) {
    oprintf(stream, "Date     : %s " CRLF, __DATE__);
    oprintf(stream, "Time     : %s " CRLF, __TIME__);
    oprintf(stream, "TimeStamp: %s " CRLF, __TIMESTAMP__);
    oprintf(stream, "Cstd     : %u " CRLF, __STDC__);
    oprintf(stream, "STDC_VER : %u " CRLF, __STDC_VERSION__);
    oprintf(stream, "__TI_COMPILER_VERSION__     : %s " CRLF, __TI_COMPILER_VERSION__);
    oprintf(stream, "board    : %s " CRLF, BOARD_NAME);
    oprintf(stream, "MCU: %s" CRLF, MCU_NAME);
    //  oprintf(stream, "IAR_SYSTEMS_ICC %u " CRLF, __IAR_SYSTEMS_ICC__);
    // oprintf (stream, "ARM_BUILD %u ", ARM_BUILD);
    // oprintf(stream, "IAR VER  :%u " CRLF, __VER__);

#ifdef __GNUC__
    oputs(stream, " GCC" CRLF);
#endif

#ifdef HAS_RELEASE
    oputs(stream, " Release" CRLF);
#endif

#ifdef HAS_DEBUG
    oputs(stream, " Debug" CRLF);
#endif
    // oprintf(stream, "Serial: 0x%" PRIX64 " " CRLF, get_device_serial());
    oputs(stream, "by aabdev" CRLF);
    oputs(stream, CRLF);
}

bool print_version(void) {
    print_version_s(DBG_STREAM);
    return true;
}

bool is_arr_pat(uint8_t* arr, uint32_t size, uint8_t patt) {
    bool res = true;
    uint32_t i;
    for(i = 0; i < size; i++) {
        if(patt != arr[i]) {
            res = false;
        }
    }
    return res;
}
extern void main(void);

void print_sys_info(void) {
    io_printf("Firmware launched.." CRLF);
    io_printf("Boot top of stack: 0x%x " CRLF, *((uint32_t*)(0x00000000)));
    io_printf("App  top of stack: 0x%x " CRLF, *((uint32_t*)(APP_START_ADDRESS)));
    io_printf("Boot reset handler: 0x%x " CRLF, *((uint32_t*)(0x00000004)));
    io_printf("App  reset handler: 0x%x " CRLF, *((uint32_t*)(APP_START_ADDRESS + 4)));
    io_printf("addr of main() 0x08%p" CRLF, main);
}

/*platform spesific data type calculator */
bool print_u16_un(U16_bit_t un) {
    io_printf("un 0x%04x 0b_%s" CRLF, un.u16, utoa_bin16(un.u16));
    io_printf("s16: %d " CRLF, un.s16);
    io_printf("u16: %u " CRLF, un.u16);
    io_printf(" u8: %u %u" CRLF, un.u8[0], un.u8[1]);
    io_printf(" s8: %d %d" CRLF, un.s8[0], un.s8[1]);
    return true;
}

bool print_16bit_types(void* val) {
    U16_bit_t union16bit;
    memcpy(&union16bit, val, 2);

    print_u16_un(union16bit);

    io_printf("Reverse bytes in %u" CRLF, union16bit.u16);

    union16bit.u16 = reverse_byte_order_uint16(union16bit.u16);
    print_u16_un(union16bit);
    return true;
}

bool print_vector_table(uint32_t vectors_table_base) {
    uint32_t* addres = 0;
    uint32_t offset = 0, num = 0;
    io_printf(CRLF "Vector table" CRLF);
    for(offset = 0, num = 0; offset <= 4 * 53; offset += 4, num++) {
        addres = (uint32_t*)(vectors_table_base + offset);
        io_printf("number %2u Addr: 0x%08p Handler: 0x%08x" CRLF, num, addres, *(addres));
    }
    return true;
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


