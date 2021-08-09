#include "i2c_commands.h"

#include <inttypes.h>

#include "convert.h"
#include "data_utils.h"
#include "debug_info.h"
#include "diag_page_nums.h"
#include "diag_report.h"
#include "i2c_drv.h"
#include "log.h"
#include "table_utils.h"

bool i2c_write_command(int32_t argc, char* argv[]) {
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

bool i2c_diag_command(int32_t argc, char* argv[]) { return false; }

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
