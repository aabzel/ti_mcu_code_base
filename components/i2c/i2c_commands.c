#include "i2c_commands.h"

#include <inttypes.h>

#include "convert.h"
#include "data_utils.h"
#include "diag_page_nums.h"
#include "diag_report.h"
#include "log.h"
#include "table_utils.h"
#include "i2c_drv.h"

bool i2c_send_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(2 == argc) {
        res = true;
        uint8_t i2c_num = 0;
        uint8_t array[256];
        uint16_t array_len = 0;
        res = try_str2uint8(argv[0], &i2c_num);
        if(false == res) {
            LOG_ERROR(I2C, "Unable to parse I2C Number [1....8]");
        }

        if(true == res) {
            res = try_str2array(argv[1], array, sizeof(array), &array_len);
            if(false == res) {
                LOG_ERROR(I2C, "Unable to extract array %s", argv[1]);
            }
        }

        if(true == res) {
            res = i2c_send(i2c_num, array, array_len);
            if(false == res) {
                LOG_ERROR(I2C, "Unable to send I2C");
            } else {
                io_printf(CRLF);
            }
        }
    } else {
        LOG_ERROR(I2C, "Usage: i2s instance hex_string");
        LOG_INFO(I2C, "instance");
        LOG_INFO(I2C, "hex_string 0x[0...F]+");
    }
    return res;
}

bool i2c_diag_command(int32_t argc, char* argv[]) {
    
    return false;
}
