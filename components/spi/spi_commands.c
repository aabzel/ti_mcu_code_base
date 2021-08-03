#include "spi_commands.h"

#include <inttypes.h>

#include "convert.h"
#include "data_utils.h"
#include "debug_info.h"
#include "diag_page_nums.h"
#include "diag_report.h"
#include "log.h"
#include "spi_drv.h"
#include "table_utils.h"

bool spi_write_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(2 == argc) {
        res = true;
        uint8_t spi_num = 0;
        uint8_t array[256];
        uint16_t array_len = 0;
        res = try_str2uint8(argv[0], &spi_num);
        if(false == res) {
            LOG_ERROR(SPI, "Unable to parse SPI Number [1....8]");
        }

        if(true == res) {
            res = try_str2array(argv[1], array, sizeof(array), &array_len);
            if(false == res) {
                LOG_ERROR(SPI, "Unable to extract array %s", argv[1]);
            }
        }

        if(true == res) {
            res = spi_write(spi_num, array, array_len);
            if(false == res) {
                LOG_ERROR(SPI, "Unable to send SPI");
            } else {
                io_printf(CRLF);
            }
        }
    } else {
        LOG_ERROR(SPI, "Usage: ss instance hex_string");
        LOG_INFO(SPI, "instance");
        LOG_INFO(SPI, "hex_string 0x[0...F]+");
    }
    return res;
}

bool spi_diag_command(int32_t argc, char* argv[]) { return false; }

bool spi_read_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(2 == argc) {
        res = true;
        uint8_t spi_num = 0;
        uint8_t array[256];
        memset(array, 0x0, sizeof(array));
        uint16_t array_len = 0;
        res = try_str2uint8(argv[0], &spi_num);
        if(false == res) {
            LOG_ERROR(SPI, "Unable to parse SPI Number [1....8]");
        }

        if(true == res) {
            res = try_str2uint16(argv[1], &array_len);
            if(false == res) {
                LOG_ERROR(SPI, "Unable to extract array %s", argv[1]);
            }
        }

        if(true == res) {
            res = spi_read(spi_num, array, array_len);
            if(false == res) {
                LOG_ERROR(SPI, "Unable to read SPI");
            } else {
                print_mem(array,array_len,false);
                io_printf(CRLF);
            }
        }
    } else {
        LOG_ERROR(SPI, "Usage: sr spi_num byte");
        LOG_INFO(SPI, "spi_num");
        LOG_INFO(SPI, "byte");
    }
    return res;
}
