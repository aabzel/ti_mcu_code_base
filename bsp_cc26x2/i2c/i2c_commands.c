#include "i2c_commands.h"

#include <i2c.h>
#include <inttypes.h>

#include "convert.h"
#include "data_utils.h"
#include "debug_info.h"
#include "diag_page_nums.h"
#include "diag_report.h"
#include "i2c_drv.h"
#include "log.h"
#include "none_blocking_pause.h"
#include "table_utils.h"

bool i2c_write_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(3 == argc) {
        res = true;
        uint8_t i2c_num = 0;
        uint8_t i2c_addr = 0;
        uint8_t array[256];
        uint32_t array_len = 0;
        if(true == res) {
            res = try_str2uint8(argv[0], &i2c_num);
            if(false == res) {
                LOG_ERROR(I2C, "Unable to parse I2C Number [1....8]");
            }
        }

        if(true == res) {
            res = try_str2uint8(argv[1], &i2c_addr);
            if(false == res) {
                LOG_ERROR(I2C, "Unable to parse I2C Number [1....8]");
            }
        }

        if(true == res) {
            res = try_str2array(argv[2], array, sizeof(array), &array_len);
            if(false == res) {
                LOG_ERROR(I2C, "Unable to extract array %s", argv[2]);
            }
        }

        if(true == res) {
            res = i2c_write(i2c_num, i2c_addr, array, array_len);
            if(false == res) {
                LOG_ERROR(I2C, "Unable to send I2C");
            } else {
                io_printf(CRLF);
            }
        }
    } else {
        LOG_ERROR(I2C, "Usage: i2s instance i2c_addr hex_string");
        LOG_INFO(I2C, "instance");
        LOG_INFO(I2C, "i2c_addr");
        LOG_INFO(I2C, "hex_string 0x[0...F]+");
    }
    return res;
}
static bool parse_i2c_mcr_reg(uint32_t i2c_mcr) {
    io_printf(CRLF "Slave mode is %sabled" CRLF, ((1 << 5) == (i2c_mcr & (1 << 5))) ? "en" : "dis");
    io_printf("Master mode is %sabled" CRLF, ((1 << 4) == (i2c_mcr & (1 << 4))) ? "en" : "dis");
    io_printf("%s operation" CRLF, ((1 << 0) == (i2c_mcr & (1 << 0))) ? "Loopback" : "Normal");
    return true;
}

static bool parse_i2c_mstat_reg(uint32_t i2c_mstat) {
    io_printf(CRLF "The I2C bus is %s" CRLF, ((1 << 6) == (i2c_mstat & (1 << 6))) ? "busy" : "idle");
    io_printf("The I2C controller is %s idle" CRLF, ((1 << 5) == (i2c_mstat & (1 << 5))) ? "" : "Not");
    io_printf("%s arbitration" CRLF, ((1 << 4) == (i2c_mstat & (1 << 4))) ? "lost" : "won");
    io_printf("The transmitted data was %s acknowledged" CRLF, ((1 << 3) == (i2c_mstat & (1 << 3))) ? "not" : "");
    io_printf("The transmitted address was %s acknowledged" CRLF, ((1 << 2) == (i2c_mstat & (1 << 2))) ? "not" : "");
    io_printf("%s" CRLF, ((1 << 1) == (i2c_mstat & (1 << 1))) ? "Error" : "");
    io_printf("The controller is %s" CRLF, ((1 << 0) == (i2c_mstat & (1 << 0))) ? "busy" : "idle");
    return true;
}

bool i2c_diag_command(int32_t argc, char* argv[]) {
    bool res;
    uint32_t* i2c_mcr = (uint32_t*)I2C_MCR_REG;
    res = parse_i2c_mcr_reg(*i2c_mcr);

    uint32_t* i2c_mstat = (uint32_t*)I2C_MSTAT_REG;
    res = parse_i2c_mstat_reg(*i2c_mstat);
    io_printf("i2c_it_cnt %u" CRLF, i2c_it_cnt);

    return res;
}
// i2r 0 0xef 1  -- hang on
// i2r 0 0xef 2  -- hang on
bool i2c_read_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(3 == argc) {
        res = true;
        uint8_t i2c_num = 0;
        uint8_t i2c_addr = 0;
        uint8_t array[256];
        uint16_t array_len = 0;
        if(true == res) {
            res = try_str2uint8(argv[0], &i2c_num);
            if(false == res) {
                LOG_ERROR(I2C, "Unable to parse I2C Number [1....8]");
            }
        }

        if(true == res) {
            res = try_str2uint8(argv[1], &i2c_addr);
            if(false == res) {
                LOG_ERROR(I2C, "Unable to parse I2C Number [1....8]");
            }
        }

        if(true == res) {
            res = try_str2uint16(argv[2], &array_len);
            if(false == res) {
                LOG_ERROR(I2C, "Unable to extract array_len %s", argv[2]);
            }
        }

        if(true == res) {
            res = i2c_read(i2c_num, i2c_addr, array, array_len);
            if(false == res) {
                LOG_ERROR(I2C, "Unable to send I2C");
            } else {
                print_mem(array, array_len, false);
                io_printf(CRLF);
            }
        }
    } else {
        LOG_ERROR(I2C, "Usage: i2r instance i2c_addr size");
        LOG_INFO(I2C, "instance");
        LOG_INFO(I2C, "i2c_addr");
        LOG_INFO(I2C, "size");
    }
    return res;
}

bool i2c_scan_command(int32_t argc, char* argv[]) {
    bool res = false;
    uint8_t i2c_num = 0;
    uint16_t array_len = 0;
    if(0 == argc) {
        res = true;
        i2c_num = 0;
        array_len = 0;
    }
    if(1 <= argc) {
        res = true;
        if(true == res) {
            res = try_str2uint8(argv[0], &i2c_num);
            if(false == res) {
                LOG_ERROR(I2C, "Unable to parse I2C Number");
            }
        }
    }
    if(2 <= argc) {
        res = true;
        if(true == res) {
            res = try_str2uint16(argv[1], &array_len);
            if(false == res) {
                LOG_ERROR(I2C, "Unable to parse array_len");
            }
        }
    }

    if(2 < argc) {
        LOG_ERROR(I2C, "Usage: i2s instance len");
        LOG_INFO(I2C, "instance");
        res = false;
    }
    if(res) {
        uint8_t array[2] = {0xFF, 0xFF};
        memset(array, 0xFF, sizeof(array));
        uint16_t i2c_addr = 0;
        for(i2c_addr = 0; i2c_addr <= 0xFF; i2c_addr++) {
            io_printf("check 0x%02x " CRLF, i2c_addr);
            res = i2c_check_addr((uint8_t)i2c_addr);
            if(false == res) {
                // LOG_ERROR(I2C, "Unable to send I2C");
            } else {
                io_printf("0x%02x " CRLF, i2c_addr);
            }
            wait_in_loop_ms(100);
        }
    }
    return res;
}
// i2cc 0xEF
// i2cc 0xaa
bool i2c_check_addr_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(1 == argc) {
        res = true;
        uint8_t i2c_addr = 0;
        if(true == res) {
            res = try_str2uint8(argv[0], &i2c_addr);
            if(false == res) {
                LOG_ERROR(I2C, "Unable to parse I2C addr %s", argv[0]);
            }
        }

        if(true == res) {
            res = i2c_check_addr(i2c_addr);
            if(false == res) {
                LOG_ERROR(I2C, "Unknown I2C addr");
            } else {
                LOG_INFO(I2C, "addr OK!");
            }
        }
    } else {
        LOG_ERROR(I2C, "Usage: i2cc i2c_addr size");
        LOG_INFO(I2C, "i2c_addr");
    }
    return res;
}

bool i2c_start_stop_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(1 == argc) {
        res = true;
        uint32_t signal = 0;
        if(true == res) {
            res = try_str2uint32(argv[0], &signal);
            if(false == res) {
                LOG_ERROR(I2C, "Unable to parse cmd %s", argv[0]);
            }
        }
        if(res) {
            I2CMasterControl(I2C0_BASE, signal);
#if 0
            res = i2c_send_signal(signal);
            if(false==res){
                LOG_ERROR(I2C, "signal error");
            }
#endif
        }
    } else {
        LOG_ERROR(I2C, "Usage: i2ss signal");
        LOG_INFO(I2C, "signal");
    }
    return res;
}

bool i2c_init_command(int32_t argc, char* argv[]) {
    bool res = i2c_init();
    if(false == res) {
        LOG_ERROR(I2C, "i2c_init error");
    }
    return res;
}
