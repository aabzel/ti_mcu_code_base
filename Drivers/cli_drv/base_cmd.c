#include "base_cmd.h"

#include <ctype.h>
#include <inttypes.h>
#include <string.h>

#include "array.h"
#include "bit_utils.h"
#include "boot_cfg.h"
#include "cli_manager.h"
#include "clocks.h"
#include "convert.h"
#include "data_utils.h"
#include "debug_info.h"
#include "diag_sys.h"
#include "io_utils.h"
#include "flash_drv.h"
#include "log.h"
#include "none_blocking_pause.h"
#include "oprintf.h"
#include "read_mem.h"
#include "str_utils.h"
#include "system.h"
#include "uart_string_reader.h"

#ifdef HAS_WDT
#include "watchdog_drv.h"
#endif /*HAS_WDT*/

#ifdef HAS_TIM
#include "tim_drv.h"
#endif /*HAS_TIM*/

#include "terminal_codes.h"
#include "version.h"


bool user_mode = true;

bool cmd_help(int32_t argc, char* argv[]) {
    bool res = false;
    io_printf("argc %u " CRLF, argc);
    if(3 <= argc) {
        io_printf("Usage: help [subname1]  " CRLF);
        io_printf("       help [subname1]  [subname2]" CRLF);
        res = false;
    } else {
        if(0 == argc) {
            help_dump_key(NULL, NULL);
            res = true;
        }
        if(1 == argc) {
            help_dump_key(argv[0], NULL);
            res = true;
        }
        if(2 == argc) {
            help_dump_key(argv[0], argv[1]);
            res = true;
        }
    }

    return res;
}

//  bc 0xxxxxx s bit
//  bc 0xxxxxx r bit
bool cmd_low_level_control(int32_t argc, char* argv[]) {
    bool res = false;
    if(3 == argc) {
        uint8_t bit_num = 0xFF;
        uint32_t address = 0u;
        char cmd = 'n';
        res = try_str2uint32(argv[0], &address);
        if(false == res) {
            LOG_ERROR(SYS, "Invalid address hex value %s", argv[0]);
        } else {
            io_printf("address: 0x%08x" CRLF, (unsigned int)address);
        }

        if(true == res) {
            cmd = argv[1][0];
            io_printf("cmd: %c" CRLF, cmd);
        }
        if(true == res) {
            res = try_str2uint8(argv[2], &bit_num);
            if(false == res) {
                LOG_ERROR(SYS, "Invalid bit_num%s", argv[2]);
            } else {
                io_printf("bit: %u" CRLF, bit_num);
            }
        }
        if(res) {
            res = bit32_control_proc((uint32_t*)address, cmd, bit_num);
            if(false == res) {
                LOG_ERROR(SYS, "Invalid address hex value %s", argv[0]);
            } else {
                io_printf("bit_control_proc OK" CRLF);
            }
        }

    } else {
        LOG_ERROR(SYS, "Usage: bit_ctrl: address cmd bit");
        LOG_INFO(SYS, "address 0xXXXXXXXX");
        LOG_INFO(SYS, "cmd [s r t]");
        LOG_INFO(SYS, "bit [0...31]");
    }
    return res;
}

bool cmd_write_memory(int32_t argc, char* argv[]) {
    bool res = false;
    if(2 == argc) {
        res = true;
        uint32_t address = 0u;
        uint32_t value = 0u;
        res = try_str2uint32(argv[0], &address);
        if(false == res) {
            LOG_ERROR(SYS, "Invalid address hex value %s", argv[0]);
        } else {
            io_printf("address: 0x%08x" CRLF, (unsigned int)address);
        }

        res = try_str2uint32(argv[1], &value);
        if(false == res) {
            LOG_ERROR(SYS, "Invalid value %s", argv[1]);
        } else {
            io_printf("val: 0x%08x" CRLF, (unsigned int)value);
        }

        if(true == res) {
            if(false==is_flash_addr(address)){
                res = write_addr_32bit(address, value);
            }
        }
    } else {
        LOG_ERROR(SYS, "Usage: wm: address value");
        LOG_INFO(SYS, "Usage: address 0xXXXXXXXX");
        LOG_INFO(SYS, "Usage: value 0xXXXXXXXX");
    }
    return res;
}

/* variable address can be obtained from *.elf due to readelf tool*/
bool cmd_read_memory(int32_t argc, char* argv[]) {
    bool res = false;
    char dir='f';
    uint32_t num_of_byte = 4u;
    uint32_t address = 0u;
    if(1 <= argc){
        res = try_str2uint32(argv[0], &address);
        if(false == res) {
            LOG_ERROR(SYS, "InvalidAddrHexVal %s", argv[0]);
        } else {
            io_printf("address: 0x%08x" CRLF, (unsigned int)address);
        }

    }
#if 0
    if(1 == argc) {
        uint32_t value = 0u;
        if(true == res) {
            value = read_addr_32bit(address);
            io_printf("value: 0x%08x" CRLF, (unsigned int)value);
            print_bit_hint(9,32);
            io_printf("value: 0b%s" CRLF, utoa_bin32(value));
            print_bit_representation(value);

        }
    }
#endif

    if(2 <= argc) {
        res = try_str2uint32(argv[1], &num_of_byte);
        if(false == res) {
            LOG_ERROR(SYS, "InvalidNumByte %s", argv[1]);
        } else {
            io_printf("numOfByte: %d " CRLF, (unsigned int)num_of_byte);
        }
    }

    if(3<=argc){
        dir=argv[2][0];
    }

    if(res){
        uint32_t offset = 0,cnt=0;
        uint8_t value_byte = 0u;
        for(cnt = 0; cnt < num_of_byte; cnt++) {
            value_byte = read_addr_8bit(address + offset);
            io_printf("%02x", (unsigned int)value_byte);
            if('f'==dir){
                offset++;
            }else if('r'==dir){
                offset--;
            }
        }
        io_printf(CRLF);

    }

    if((0 == argc) || ( 3 < argc)) {
        LOG_ERROR(SYS, "Usage: read_mem: address num_of_byte dir");
        LOG_INFO(SYS, "Usage: address 0xXXXXXXXX");
        LOG_INFO(SYS, "Usage: num_of_byte [0...]");
        LOG_INFO(SYS, "Usage: dir [f r]");
    }
    return res;
}

bool cmd_sysinfo(int32_t argc, char* argv[]) {
    (void)argv;
    bool res = false;
    if(argc != 0) {
        LOG_ERROR(SYS, "Usage: sysinfo");
        res = false;
    } else {
        // io_printf ("up time: %u [ms] / %u [s]" CRLF, g_up_time_ms,
        // g_up_time_ms / 1000);
        explore_stack_dir();
        print_sysinfo();
        if(is_little_endian()) {
            io_printf("Little endian" CRLF);
        } else {
            io_printf("big endian" CRLF);
        }
        res = true;
    }

    return res;
}

bool cmd_wd_test(int32_t argc, char* argv[]) {
    uint32_t delay_in_ms = 1000;
    if(argc != 1) {
        LOG_ERROR(SYS, "Usage: wd_test time_ms");
        return false;
    }
    if(!try_str2uint32(argv[0], &delay_in_ms)) {
        LOG_ERROR(SYS, "Invalid delay value %s", argv[0]);
        return false;
    }
    LOG_WARNING(SYS, "Delay for %" PRIu32 " ms", delay_in_ms);
    flush_printf();
    delay_ms(delay_in_ms);
    return true;
}

bool cmd_wd_test_hw(int32_t argc, char* argv[]) {
    uint32_t delay_in_ms = 1000;
    if(argc != 1) {
        LOG_ERROR(SYS, "Usage: wd_test_hw time_ms");
        return false;
    }
    if(!try_str2uint32(argv[0], &delay_in_ms)) {
        LOG_ERROR(SYS, "Invalid delay value %s", argv[0]);
        return false;
    }
    LOG_WARNING(SYS, "Delay for %" PRIu32 " ms", delay_in_ms);
    flush_printf();
    {
        uint32_t end_time = get_time_ms32() + delay_in_ms;
        while(get_time_ms32() <= end_time) {
#ifdef HAS_SWT
            wdt_reset_sw();
#endif
        }
    }
    return true;
}

void shell_prompt(void) {
    if(cli_echo) {
        io_printf("-->");
    }
}

bool cmd_version(int32_t argc, char* argv[]) {
    (void)(argv);
    if(argc != 0) {
        LOG_ERROR(SYS, "Usage: version");
        return false;
    }
    if(!user_mode) {
        io_printf("!VI");
    }
    print_version();
    return true;
}

bool dump_cmd_result(bool res) {
#ifndef EMBEDDED_TEST
    if(user_mode) {
        if(res) {
            LOG_INFO(SYS, "Ok!");
        } else {
            LOG_ERROR(SYS, "Failed!");
        }
    } else {
#endif
        if(res) {
            io_putstr("!OK" CRLF);
        } else {
            io_putstr("!ERROR" CRLF);
        }
#ifndef EMBEDDED_TEST
    }
#endif
    return res;
}

bool dump_cmd_result_ex(bool res, const char* message) {
    if(user_mode) {
        if(res) {
            LOG_INFO(SYS, "Ok: %s", message);
        } else {
            LOG_ERROR(SYS, "Failed: %s", message);
        }
    } else {
        if(res) {
            io_printf("!OK %s" CRLF, message);
        } else {
            io_printf("!ERROR %s" CRLF, message);
        }
    }
    return res;
}

#define MAX_RPT_CMD_LEN 50U
bool cmd_repeat(int32_t argc, char* argv[]) {
    bool res = false;
    if(3 == argc) {
        char read_command[MAX_RPT_CMD_LEN] = "";
        char temp_command[MAX_RPT_CMD_LEN] = "";
        res = true;
        uint32_t period_ms=0;
        uint32_t num_of_try=0;
        strncpy(read_command, argv[0], sizeof(read_command));
        if(true == res) {
            res = try_str2uint32(argv[1], &period_ms);
            if(false == res) {
                LOG_ERROR(SYS, "Unable to parse period_ms", argv[1]);
            }
        }
        if(true == res) {
            res = try_str2uint32(argv[2], &num_of_try);
            if(false == res) {
                LOG_ERROR(SYS, "Unable to parse num_of_try", argv[2]);
            }
        }
        uint32_t iter = 0U;
        replace_char(read_command, '_', ' ');
        for(iter = 1; iter <= num_of_try; iter++) {
            io_printf(CRLF"%03u/%u: execute command [%s]" CRLF, iter, num_of_try, read_command);
            strncpy(temp_command, read_command, sizeof(temp_command));
            process_shell_cmd(temp_command);
#ifdef NORTOS
            wait_in_loop_ms(period_ms);
#endif /*NORTOS*/
        }
    } else {
        LOG_ERROR(SYS, "Usage: rpt command period_ms times");
    }
    return res;
}


bool cmd_launch_function(int32_t argc, char* argv[]) {
    bool res = false;
    if(1 == argc) {
        res = true;
        uint32_t function_address = 0;
        void (*function_ptr)(void);
        if(true == res) {
            res = try_str2uint32(argv[0], &function_address);
            if(false == res) {
                LOG_ERROR(SYS, "Unable to parse address %s", argv[0]);
            }
        }
        if(true == res) {
            function_ptr = (void (*)(void))function_address;
            function_ptr();
        }

    } else {
        LOG_ERROR(SYS, "Usage: lfun func_address_hex");
    }
    return res;
}

bool cmd_find_addr(int32_t argc, char* argv[]) {
    bool res = false;
    if(3 == argc) {
        res = true;
        uint16_t byte_num = 0;
        uint32_t val = 0;
        uint8_t mem = 0;
        if(true == res) {
            res = try_str2uint16(argv[0], &byte_num);
            if(false == res) {
                LOG_ERROR(SYS, "Unable to byte_num %s", argv[0]);
            }
        }

        if(true == res) {
            res = try_str2uint32(argv[1], &val);
            if(false == res) {
                LOG_ERROR(SYS, "Unable to parse value %s", argv[1]);
            }
        }

        if(true == res) {
            res = try_str2uint8(argv[2], &mem);
            if(false == res) {
                LOG_ERROR(SYS, "Unable to parse mem %s", argv[2]);
            }
        }

        if(true == res) {
            if(0 == mem) {
                res = find_addr_by_val(byte_num, val, RAM_START, RAM_END);
                if(false == res) {
                    LOG_ERROR(SYS, "Unable to find addr for value %u", byte_num, val);
                }
            }
            if(1 == mem) {
                res = find_addr_by_val(byte_num, val, ROM_START, ROM_END);
                if(false == res) {
                    LOG_ERROR(SYS, "Unable to find addr for value %u", byte_num, val);
                }
            }
        }
    } else {
        LOG_ERROR(SYS, "Usage: faddr val_len value mem");
        LOG_INFO(SYS, "val_len [1 2 4]");
        LOG_INFO(SYS, "byte_num 32bitval");
        LOG_INFO(SYS, "mem [0-RAM, 1-Flash]");
    }
    return res;
}

/*calc -179*/
bool cmd_calc(int32_t argc, char* argv[]) {
    bool res = false;
    if(1 == argc) {
        res = true;
        uint64_t value64u = 0;
        res = try_str2uint64(argv[0], &value64u);
        if(true == res) {
            io_printf("spot uint64 %llu" CRLF, value64u);
            print_64bit_types((void*)&value64u);
        }

        uint16_t value16u = 0;
        res = try_str2uint16(argv[0], &value16u);
        if(true == res) {
            io_printf("spot uint16 %u" CRLF, value16u);
            print_16bit_types((void*)&value16u);
        }

        int16_t value16i = 0;
        res = try_str2int16(argv[0], &value16i);
        if(true == res) {
            io_printf("spot int16 %d" CRLF, value16i);
            print_16bit_types((void*)&value16i);
        }

    } else {
        LOG_ERROR(SYS, "Usage: lfun func_address_hex");
    }
    return res;
}

bool cmd_ascii(int32_t argc, char* argv[]) {
    bool res = false;
    if(0 == argc) {
        res = true;
        uint16_t byte = 0;
        for(byte = 0; byte <= 0xFF; byte++) {
            io_printf("0x%02x %d [%c]" CRLF, byte, byte, byte);
        }
    } else if(1 == argc) {
        res = true;
        uint8_t byte = 0;
        res = try_str2uint8(argv[0], &byte);
        if(false == res) {
            LOG_ERROR(SYS, "unable to read value %s", argv[0]);
        }
        if(true == res) {
            io_printf("0x%02x %d [%c]" CRLF, byte, byte, byte);
        }
    } else {
        LOG_ERROR(SYS, "Usage: ascii [byte]");
    }
    return res;
}

bool cmd_echo(int32_t argc, char *argv []) {
    bool echo = false;
    bool res = false;
    if(0 == argc){
        res = cli_toggle_echo();
    }else if (1==argc) {
        res = try_str2bool(argv[0], &echo);
        if (false == res) {
            LOG_ERROR(SYS, "Unable parse status %s",argv[0]);
        }
        res = cli_set_echo(echo);
        if ( res ) {
            LOG_INFO(SYS, "OK");
        }
    } else {
        LOG_ERROR(SYS, "Usage: echo state");
    }

    return res;
}

