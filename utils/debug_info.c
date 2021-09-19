#include "debug_info.h"

#include <inttypes.h>
#include <string.h>

#include "byte_utils.h"
#include "convert.h"
#ifndef USE_HAL_DRIVER
#include "core_driver.h"
#endif
#include "crc32.h"
#include "data_utils.h"
#include "device_id.h"
#ifdef HAS_FLASH
#include "flash_drv.h"
#endif
#include "io_utils.h"
#include "oprintf.h"
#include "sys.h"
#include "table_utils.h"
#include "uart_drv.h"
#include "version.h"
#include "writer_generic.h"

extern int main(void);
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

bool print_version_s(ostream_t* stream) {
    bool res = false;
    if(stream) {
        res = false;
        uint32_t all_flash_crc = 0;
        oprintf(stream, "Date     : %s " CRLF, __DATE__);
        oprintf(stream, "Time     : %s " CRLF, __TIME__);
        oprintf(stream, "TimeStamp: %s " CRLF, __TIMESTAMP__);
        oprintf(stream, "Cstd     : %u " CRLF, __STDC__);
        oprintf(stream, "STDC_VER : %u " CRLF, __STDC_VERSION__);
        //  oprintf(stream, "__TI_COMPILER_VERSION__     : %s " CRLF, __TI_COMPILER_VERSION__);
        oprintf(stream, "board    : %s " CRLF, BOARD_NAME);
        oprintf(stream, "MCU: %s" CRLF, MCU_NAME);
#ifndef USE_HAL_DRIVER
        uint32_t cpi_id = cpu_get_id();
#endif /*USE_HAL_DRIVER*/
        oprintf(stream, "branch   : %s " CRLF, GIT_BRANCH);
        oprintf(stream, "last commit: %s" CRLF, GIT_LAST_COMMIT_HASH);
#ifdef HAS_FLASH
        all_flash_crc = crc32(((uint8_t*)NOR_FLASH_BASE), NOR_FLASH_SIZE);
#endif /*HAS_FLASH*/
        oprintf(stream, "FlashCRC32: 0x%08X" CRLF, all_flash_crc);
        oprintf(stream, "main(): 0x%08p" CRLF, main);
#ifdef __TI_COMPILER_VERSION__
        oprintf(stream, "TI compiler ver %u " CRLF, __TI_COMPILER_VERSION__);
#endif

#ifdef __GNUC__
        oputs(stream, "GCC" CRLF);
#endif /**/

#ifdef HAS_BOOTLOADER
        oputs(stream, "Bootloader ");
#endif /*HAS_BOOTLOADER*/

#ifdef HAS_GENERIC
        oputs(stream, "Generic ");
#endif /*HAS_GENERIC*/

#ifdef HAS_RELEASE
        oputs(stream, "Release" CRLF);
#endif /*HAS_RELEASE*/

#ifdef HAS_DEBUG
        oputs(stream, "Debug" CRLF);
#endif /*HAS_DEBUG*/

#ifndef USE_HAL_DRIVER
        oprintf(stream, "Serial: 0x%" PRIX64 " " CRLF, get_device_serial());
        uint64_t ble_mac = get_ble_mac();
        oprintf(stream, "MAC: 0x%" PRIX64 CRLF, ble_mac);
#endif /*USE_HAL_DRIVER*/
        oputs(stream, "by aabdev" CRLF);
        oputs(stream, CRLF);
    }
    return res;
}

bool print_version(void) {
    bool res = false;
    res = print_version_s(DBG_STREAM);
    return res;
}

void print_sys_info(void) {
    uint32_t top_stack_val = *((uint32_t*)(APP_START_ADDRESS));
    io_printf("Boot top of stack: 0x%x " CRLF, *((uint32_t*)(0x00000000)));
    io_printf("App  top of stack: 0x%x " CRLF, top_stack_val);
    io_printf("Boot reset handler: 0x%x " CRLF, *((uint32_t*)(0x00000004)));
    io_printf("App  reset handler: 0x%x " CRLF, *((uint32_t*)(APP_START_ADDRESS + 4)));
    io_printf("addr of main() 0x08%p" CRLF, main);
    explore_stack_dir();
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
#define ASCII_SEP "|"
bool print_ascii_line(char* buff, uint16_t size, uint16_t indent) {
    uint16_t i = 0;
    bool res = false;
    res = print_indent(indent);
    io_printf(ASCII_SEP);
    for(i = 0; i < size; i++) {
        if(0x08 == buff[i]) {
            io_printf("[BS]");
        } else if(0x00 == buff[i]) {
            io_printf(" ");
        } else if(0x0A == buff[i]) {
            io_printf("[LF]");
        } else if(0x0D == buff[i]) {
            io_printf("[CR]");
        } else {
            io_printf("%c", buff[i]);
        }
    }
    io_printf(ASCII_SEP);
    return res;
}

bool print_bin(uint8_t* buff, uint16_t size, uint16_t indent) {
    uint16_t i = 0;
    bool res = false;
    res = print_indent(indent);
    for(i = 0; i < size; i++) {
        io_printf("%02x", buff[i]);
    }
    return res;
}

bool print_mem(uint8_t* addr, uint32_t len, bool new_line) {
    bool res = false;
    uint32_t pos = 0;
    uint32_t print_len = 0;
    int32_t rem;
    uint8_t hexLine[16 * 2 + 1];
    memset(hexLine, 0x00, sizeof(hexLine));
    if(16 < len) {
        for(pos = 0; pos < (len - 16); pos += 16) {
            res = true;
            hex2ascii(&addr[pos], 16, hexLine, (uint32_t)sizeof(hexLine));
            io_printf("%s", hexLine);
            print_ascii_line((char*)&addr[pos], 16, 4);
            print_len += 16;
            io_printf(CRLF);
        }
    }
    rem = len - print_len;

    if(0 < rem) {
        memset(hexLine, 0x00, sizeof(hexLine));
        res = true;
        pos = len / 16;
        hex2ascii(&addr[print_len], rem, hexLine, sizeof(hexLine));
        io_printf("%s", hexLine);
        print_ascii_line((char*)&addr[print_len], rem, 2 * (16 - rem) + 4);
    }
    if(true == new_line) {
        io_printf(CRLF);
    }
    return res;
}

bool print_mem2(uint8_t* addr, uint32_t len, bool new_line) {
    io_printf("0x");
    bool res = false;
    char asciiLine[18];
    uint8_t char_pos = 0;
    memset(asciiLine, 0x00, sizeof(asciiLine));
    if(0 < len) {
        res = true;
        uint32_t pos = 0;
        for(pos = 0; pos < len; pos++) {
            if(char_pos < 16) {
                asciiLine[char_pos] = *(addr + pos);
                char_pos++;
                if(16 == char_pos) {
                    //    char_pos = 0;
                }
            }
            if(0 == (pos % 16)) {
                if(pos) {
                    print_ascii_line(asciiLine, sizeof(asciiLine), 4);
                    memset(asciiLine, 0x00, sizeof(asciiLine));
                    char_pos = 0;
                }
                io_printf(CRLF);
            }
            io_printf("%02x", *(addr + pos));
        }
    }
    if(len < 16) {
        print_ascii_line(asciiLine, sizeof(asciiLine), 4);
    }
    if(true == new_line) {
        io_printf(CRLF);
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

bool print_offset(uint16_t offset) {
    bool res = false;
    if(0 < offset) {
        res = true;
        uint16_t i = 0;
        for(i = 0; i < offset; i++) {
            io_printf(" ");
        }
    }
    return res;
}

bool print_bit_hint(uint16_t offset, uint32_t bitness) {
    int32_t bit = 0;
    uint8_t cnt = 0;
    bool res = false;
    if(bitness) {
        res = true;
        print_offset(offset);
        for(bit = bitness - 1; 0 <= bit; bit--) {
            io_printf("%u", bit / 10);
            cnt++;
            if(4 == cnt) {
                cnt = 0;
                io_printf(" ");
            }
        }
        cnt = 0;
        io_printf(CRLF);
        print_offset(offset);
        for(bit = bitness - 1; 0 <= bit; bit--) {
            io_printf("%u", bit % 10);
            cnt++;
            if(4 == cnt) {
                cnt = 0;
                io_printf(" ");
            }
        }
        io_printf(CRLF);
    }
    return res;
}

bool print_bit_representation(uint32_t val) {
    bool res = true;
    int32_t bit_index = 0;
    table_col_t cols[32];
    for(bit_index = 31; 0 <= bit_index; bit_index--) {
        cols[bit_index].width = 2;
        cols[bit_index].name = "";
    }
    io_printf("value: %u" CRLF "value: 0x%x " CRLF, val, val);
    table_row_bottom(&dbg_o.s, cols, ARRAY_SIZE(cols));
    io_printf(TSEP);
    for(bit_index = 31; 0 <= bit_index; bit_index--) {
        io_printf("%2u" TSEP, bit_index);
    }
    io_printf(CRLF);
    table_row_bottom(&dbg_o.s, cols, ARRAY_SIZE(cols));
    io_printf(TSEP);

    for(bit_index = 31; 0 <= bit_index; bit_index--) {
        io_printf("%2u" TSEP, (1 << bit_index) == (val & (1 << bit_index)));
    }
    io_printf(CRLF);
    table_row_bottom(&dbg_o.s, cols, ARRAY_SIZE(cols));
    io_printf(CRLF);
    return res;
}

static bool print_text_addresses(uint32_t cur_stack_val, uint32_t top_stack_val) {
    bool res = false;
    bool out_res = false;
    uint32_t cur_addr = 0;
    uint32_t num = 1;
    if(cur_stack_val < top_stack_val) {
        for(cur_addr = cur_stack_val; cur_addr < top_stack_val; cur_addr++) {
            uint32_t* ram_addr = (uint32_t*)cur_addr;
            res = is_flash_addr((uint32_t)*ram_addr);
            if(res) {
                io_printf("%3u addr[0x%08x]=0x%08x" CRLF, num, cur_addr, (uint32_t)*ram_addr);
                out_res = true;
                num++;
            }
        }
    }
    return out_res;
}

bool parse_stack(void) {
    bool res = false;
    uint32_t cur_stack_val = (uint32_t)&res;
    uint32_t top_stack_val = *((uint32_t*)(APP_START_ADDRESS));
    uint32_t cur_stack_size = top_stack_val - cur_stack_val;
    io_printf("Stack 0x%08x...0x%08x %u byte %u kByte" CRLF, cur_stack_val, top_stack_val, cur_stack_size,
              cur_stack_size / 1024);
    res = print_text_addresses(cur_stack_val, top_stack_val);

    return res;
}
