#include "debug_info.h"

#include <inttypes.h>
#include <string.h>

#include "byte_utils.h"
#include "convert.h"
#ifdef HAS_CRC
#include "crc32.h"
#endif
#include "data_utils.h"
#include "io_utils.h"

#ifdef STM32F413xx
#include "cmsis_os.h"
#include "stm32f4xx_hal.h"
#endif /*STM32F413xx*/

#ifdef HAS_UART
#include "uart_drv.h"
#endif
#ifdef HAS_FLASH
#include "flash_drv.h"
#endif

#ifdef HAS_DEV_ID
#include "device_id.h"
#endif

#ifdef HAS_LOG
#include "table_utils.h"
#include "writer_config.h"
#include "writer_generic.h"
#include "oprintf.h"
#endif

#ifdef HAS_MCU
#include "sys_config.h"
#ifndef USE_HAL_DRIVER
#include "core_driver.h"
#endif
#ifdef HAS_BOOT
#include "boot_cfg.h"
#endif
#include "version.h"
#endif

#ifdef X86_64
#include <stdio.h>
#define io_printf printf
#endif
#ifdef NORTOS
extern int main(void);
#endif
static bool stack_dir(int32_t* main_local_addr) {
    bool res = false;
    int32_t fun_local = 0;
    if(((void*)main_local_addr) < ((void*)&fun_local)) {
        io_printf("Stack grows from small addr to big addr -> \n" CRLF);
    } else {
        io_printf("Stack grows from big addr to small addr <- \n" CRLF);
    }
    return res;
}

bool explore_stack_dir(void) {

    bool res = false;
    int32_t main_local;
    res = stack_dir(&main_local);
    return res;
}

#if 0
static bool is_big_endian(void) {
    U32_bit_t bint;
    bint.u32 = 0x01020304;
    return bint.u8[0] == 1;
}
#endif

bool is_little_endian(void) {
    U32_bit_t bint;
    bint.u32 = 0x01020304;
    return bint.u8[0] == 4;
}

#ifdef HAS_LOG
static bool print_fw_type(void) {
    bool res = false;
    io_printf("config: %s " CRLF, CONFIG_NAME);
#ifdef HAS_BOOTLOADER
    io_printf("Bootloader ");
#endif /*HAS_BOOTLOADER*/

#ifdef HAS_GENERIC
    io_printf("Generic ");
#endif /*HAS_GENERIC*/

#ifdef HAS_RELEASE
    io_printf("Release" CRLF);
#endif /*HAS_RELEASE*/

#ifdef HAS_DEBUG
    io_printf("Debug" CRLF);
#endif /*HAS_DEBUG*/
    res = true;
    return res;
}
#endif

#ifdef HAS_LOG
void print_sysinfo(void) {
#ifdef HAS_CORTEX_M
    io_printf("Reset handler: 0x%x " CRLF, *((uint32_t*)0x00000004));
    io_printf("top-of-stack: %x " CRLF, *((uint32_t*)0x00000000));
    io_printf("boot memory start: %x " CRLF, *((uint32_t*)0x00000004));
#endif
    // io_printf("addr of SystemInit() 0x%p" CRLF, SystemInit);
#ifdef NORTOS
    io_printf("addr of main() 0x%p" CRLF, main);
#endif
    io_printf("RAM: %u Byte" CRLF, RAM_SIZE);
    io_printf("Flash: %u Byte" CRLF, ROM_SIZE);
    io_printf("RAM addr:   0x%08x....0x%08x " CRLF, RAM_START, RAM_END);
    io_printf("Flash addr: 0x%08x....0x%08x " CRLF, ROM_START, ROM_END);
}

bool print_version(void) {
    bool res = true;
    print_fw_type();
    uint32_t all_flash_crc = 0;
    io_printf("Date: %s" CRLF, __DATE__);
    io_printf("Time: %s" CRLF, __TIME__);
    io_printf("TimeStamp: %s" CRLF, __TIMESTAMP__);
    io_printf("Cstd: %u" CRLF, __STDC__);
    io_printf("StdCver: %lu" CRLF, __STDC_VERSION__);
    io_printf("board: %s" CRLF, BOARD_NAME);
    io_printf("MCU: %s" CRLF, MCU_NAME);
#ifdef HAS_DEV_ID
    uint32_t cpi_id = cpu_get_id();
#endif /*USE_HAL_DRIVER*/
    io_printf("branch: %s" CRLF, GIT_BRANCH);
    io_printf("lastCommit: %s" CRLF, GIT_LAST_COMMIT_HASH);

#if defined(HAS_CRC) && defined(HAS_FLASH)
    all_flash_crc = crc32(((uint8_t*)NOR_FLASH_BASE), NOR_FLASH_SIZE);
#endif /*HAS_FLASH*/
    io_printf("FlashCRC32: 0x%08X" CRLF, all_flash_crc);
#ifdef NORTOS
    io_printf("main(): 0x%08p" CRLF, main);
#endif
#ifdef __TI_COMPILER_VERSION__
    io_printf("TIcompilerVer %u" CRLF, __TI_COMPILER_VERSION__);
#endif

#ifdef __GNUC__
    io_printf("GCC" CRLF);
#endif /**/

#ifdef HAS_DEV_ID
    io_printf("Serial: 0x%" PRIX64 " " CRLF, get_device_serial());
    uint64_t ble_mac = get_ble_mac();
    io_printf("MAC: 0x%" PRIX64 CRLF, ble_mac);
#endif /*HAS_DEV_ID*/
    io_printf("TG: @aabdev" CRLF);
    io_printf("Made in Russia" CRLF);

    return res;
}
#endif /*USE_HAL_DRIVER*/

#ifdef HAS_LOG

void print_sys_info(void) {
    uint32_t top_stack_val = *((uint32_t*)(APP_START_ADDRESS));
    io_printf("BootStackEnd: 0x%x " CRLF, *((uint32_t*)(0x00000000)));
    io_printf("AppStackEnd: 0x%x " CRLF, top_stack_val);
    io_printf("Boot reset handler: 0x%x " CRLF, *((uint32_t*)(0x00000004)));
    io_printf("App  reset handler: 0x%x " CRLF, *((uint32_t*)(APP_START_ADDRESS + 4)));
#ifdef NORTOS
    io_printf("addr of main() 0x08%p" CRLF, main);
#endif
    explore_stack_dir();
}
#endif /*HAS_LOG*/

/*platform spesific data type calculator */
static bool print_u16_un(U16_bit_t un) {
    io_printf("un 0x%04x 0b_%s" CRLF, un.u16, utoa_bin16(un.u16));
    io_printf("s16: %d " CRLF, un.s16);
    io_printf("u16: %u " CRLF, un.u16);
    io_printf("u8: %u %u" CRLF, un.u8[0], un.u8[1]);
    io_printf("s8: %d %d" CRLF, un.s8[0], un.s8[1]);
    return true;
}

bool print_u64_un(U64_bit_t un) {
    uint8_t i = 0;
    io_printf("un 0x%16llx 0b_%s" CRLF, un.u64, utoa_bin64(un.u64));
    io_printf("s64: %lld " CRLF, un.s64);
    io_printf("u64: %llu " CRLF, un.u64);

    for(i = 0; i < 2; i++) {
        io_printf("0x%08x ", un.u32[i]);
    }
    io_printf(CRLF);

    for(i = 0; i < 2; i++) {
        io_printf("%d ", un.s32[i]);
    }
    io_printf(CRLF);

    for(i = 0; i < 4; i++) {
        io_printf("0x%04x ", un.u16[i]);
    }
    io_printf(CRLF);

    for(i = 0; i < 4; i++) {
        io_printf("%d ", un.s16[i]);
    }
    io_printf(CRLF);

    for(i = 0; i < 8; i++) {
        io_printf("0x%02x ", un.u8[i]);
    }
    io_printf(CRLF);

    for(i = 0; i < 8; i++) {
        io_printf("%d ", un.s8[i]);
    }
    io_printf(CRLF);
    return true;
}

bool print_16bit_types(void* val) {
    U16_bit_t union16bit = {0};
    memcpy(&union16bit, val, 2);

    print_u16_un(union16bit);

    io_printf("Reverse bytes in %u" CRLF, union16bit.u16);

    union16bit.u16 = reverse_byte_order_uint16(union16bit.u16);
    print_u16_un(union16bit);
    return true;
}

bool print_64bit_types(void* val) {
    U64_bit_t union64bit = {0};
    memcpy(&union64bit.u64, val, 8);

    print_u64_un(union64bit);

    io_printf("Reverse bytes in %llu" CRLF, union64bit.u64);

    union64bit.u64 = reverse_byte_order_uint64(union64bit.u64);
    print_u64_un(union64bit);
    return true;
}

#if defined(HAS_LOG) && !defined(ESP32)
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
#endif

#define ASCII_SEP "|"
bool print_ascii_line(char* buff, uint16_t size, uint16_t indent) {
    uint16_t i = 0;
    bool res = false;
    res = print_indent(indent);
    // io_printf(ASCII_SEP);
    for(i = 0; i < size; i++) {
#ifdef HAS_REPLACE_FORMATTER_CHARACTERS
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
#else
        io_printf("%c", buff[i]);
#endif
    }
    // io_printf(ASCII_SEP);
    return res;
}

bool print_indent(uint16_t indent) {
    bool res = false;
    uint16_t i = 0;
    if((0 < indent) && (indent < 80)) {
        res = true;
        for(i = 0; i < indent; i++) {
            io_printf(" ");
        }
    }
    return res;
}

bool print_bin(uint8_t* buff, uint32_t size, uint16_t indent) {
    uint32_t i = 0;
    bool res = false;
    res = print_indent(indent);
    for(i = 0; i < size; i++) {
        io_printf("%02x", buff[i]);
    }
    return res;
}

bool print_mem(uint8_t* addr, uint32_t len, bool is_bin, bool is_ascii, bool new_line, bool is_packed) {
    bool res = false;
    uint32_t pos = 0;
    uint32_t print_len = 0;
    int32_t rem = 0;
    if(16 < len) {
        for(pos = 0; pos < (len - 16); pos += 16) {
            res = true;
            if(is_bin) {
                print_bin(&addr[pos], 16, 0);
            }
            if(is_ascii) {
                print_ascii_line((char*)&addr[pos], 16, 4);
            }
            print_len += 16;
            if(is_packed) {
                io_printf(CRLF);
            }
        }
    }
    rem = len - print_len;

    if(0 < rem) {
        res = true;
        pos = len / 16;
        if(is_bin) {
            print_bin(&addr[print_len], rem, 0);
        }
        if(is_ascii) {
            print_ascii_line((char*)&addr[print_len], rem, 2 * (16 - rem) + 4);
        }
    }
    if(true == new_line) {
        io_printf(CRLF);
    }
    return res;
}

#if 0
static bool print_mem2(uint8_t* addr, uint32_t len, bool new_line) {
    io_printf("0x");
    bool res = false;
    char asciiLine[18] = "";
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
#endif

#ifdef HAS_LOG
bool find_addr_by_val(uint16_t byte_num, uint32_t val, uint32_t start_addr, uint32_t end_addr) {
    bool res = false;
    uint32_t cnt = 0;
    if(1 == byte_num) {
        uint8_t value = (uint8_t)val;
        uint8_t* ptr = (uint8_t*)start_addr;
        for(ptr = (uint8_t*)start_addr; ptr < ((uint8_t*)end_addr); ptr++) {
            if((*ptr) == value) {
                io_printf("%u: address *(0x%p): 0x%04x/%u" CRLF, cnt, ptr, value, value);
                res = true;
            }
        }
    }

    if(2 == byte_num) {
        uint16_t value = (uint16_t)val;
        uint16_t* ptr = (uint16_t*)start_addr;
        for(ptr = (uint16_t*)start_addr; ptr < ((uint16_t*)end_addr); ptr++) {
            if((*ptr) == value) {
                io_printf("%u: addr *(0x%p): 0x%04x/%u" CRLF, cnt, ptr, value, value);
                res = true;
            }
        }
    }

    if(4 == byte_num) {
        uint32_t value = (uint32_t)val;
        uint32_t* ptr = (uint32_t*)start_addr;
        for(ptr = (uint32_t*)start_addr; ptr < ((uint32_t*)end_addr); ptr++) {
            if((*ptr) == value) {
                io_printf("%u: addr *(0x%p): 0x%08x/%u" CRLF, cnt, ptr, value, value);
                res = true;
            }
        }
    }
    return res;
}
#endif

static bool print_offset(uint16_t offset) {
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

#ifdef HAS_LOG
bool print_bit_representation(uint32_t val) {
    bool res = true;
    int32_t bit_index = 0;
    table_col_t cols[32];
    for(bit_index = 31; 0 <= bit_index; bit_index--) {
        cols[bit_index].width = 2;
        cols[bit_index].name = "";
    }
    io_printf("value: %u" CRLF "value: 0x%x " CRLF, val, val);
    table_row_bottom(&(curWriterPtr->s), cols, ARRAY_SIZE(cols));
    io_printf(TSEP);
    for(bit_index = 31; 0 <= bit_index; bit_index--) {
        io_printf("%2u" TSEP, bit_index);
    }
    io_printf(CRLF);
    table_row_bottom(&(curWriterPtr->s), cols, ARRAY_SIZE(cols));
    io_printf(TSEP);

    for(bit_index = 31; 0 <= bit_index; bit_index--) {
        io_printf("%2u" TSEP, (1 << bit_index) == (val & (1 << bit_index)));
    }
    io_printf(CRLF);
    table_row_bottom(&(curWriterPtr->s), cols, ARRAY_SIZE(cols));
    io_printf(CRLF);
    return res;
}
#endif

#ifdef HAS_FLASH
static bool print_text_addresses(uint32_t cur_stack_val, uint32_t top_stack_val) {
    bool res = false;
    bool out_res = false;
    uint32_t cur_addr = 0;
    uint32_t num = 1;
    if(cur_stack_val < top_stack_val) {
        for(cur_addr = cur_stack_val; cur_addr < (top_stack_val - 4); cur_addr++) {
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
#endif

void error_handler(void) {
#ifdef STM32F413xx
    __disable_irq();
#endif /*STM32F413xx*/
    while(1) {
    }
}

#ifdef HAS_FLASH
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
#endif
