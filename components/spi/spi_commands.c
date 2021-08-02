#include "spi_commands.h"

#include <inttypes.h>

#include "convert.h"
#include "data_utils.h"
#include "diag_page_nums.h"
#include "diag_report.h"
#include "log.h"
#include "table_utils.h"
#include "spi_drv.h"

bool spi_send_command(int32_t argc, char* argv[]) {
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
            res = spi_send(spi_num, array, array_len);
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

bool spi_diag_command(int32_t argc, char* argv[]) {
    
    return false;
}
