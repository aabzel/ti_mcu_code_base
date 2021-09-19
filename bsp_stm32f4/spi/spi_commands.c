#include "spi_commands.h"

#include <inttypes.h>
#include <stdio.h>

#include "convert.h"
#include "data_utils.h"
#include "debug_info.h"
#include "log.h"
#include "spi_drv.h"
#include "table_utils.h"

bool spi_diag_command(int32_t argc, char* argv[]) {
    bool res = false;
    uint8_t spi_num = 0;
    static const table_col_t cols[] = {{5, "No"},   {10, "clk"}, {5, "pha"}, {5, "plo"},
                                       {5, "bits"}, {8, "tx"},   {8, "rx"},  {10, "name"}};
    char temp_str[120];
    table_header(&dbg_o.s, cols, ARRAY_SIZE(cols));
    for(spi_num = 0; spi_num < SPI_CNT; spi_num++) {
        strcpy(temp_str, TSEP);
        snprintf(temp_str, sizeof(temp_str), "%s %3u " TSEP, temp_str, spi_num);
        snprintf(temp_str, sizeof(temp_str), "%s %8u " TSEP, temp_str, spi_get_clock((SpiName_t)spi_num));
        snprintf(temp_str, sizeof(temp_str), "%s  %1u  " TSEP, temp_str, spi_get_phase((SpiName_t)spi_num));
        snprintf(temp_str, sizeof(temp_str), "%s  %1u  " TSEP, temp_str, spi_get_polarity((SpiName_t)spi_num));
        snprintf(temp_str, sizeof(temp_str), "%s  %2u " TSEP, temp_str, spi_get_data_size((SpiName_t)spi_num));
        snprintf(temp_str, sizeof(temp_str), "%s %6u " TSEP, temp_str, SpiInstance[spi_num].tx_byte_cnt);
        snprintf(temp_str, sizeof(temp_str), "%s %6u " TSEP, temp_str, SpiInstance[spi_num].rx_byte_cnt);
        snprintf(temp_str, sizeof(temp_str), "%s %8s " TSEP, temp_str, SpiInstance[spi_num].name);
        snprintf(temp_str, sizeof(temp_str), "%s" CRLF, temp_str);
        io_printf("%s", temp_str);
    }
    table_row_bottom(&dbg_o.s, cols, ARRAY_SIZE(cols));

    return res;
}

bool spi_diag_int_command(int32_t argc, char* argv[]) {
    bool res = false;
    uint8_t spi_num = 0;
    static const table_col_t cols[] = {{5, "No"}, {4, "rx"}, {4, "tx"}, {6, "rxTo"}, {6, "RxOrun"}, {10, "name"}};
    char temp_str[120];
    table_header(&dbg_o.s, cols, ARRAY_SIZE(cols));
    for(spi_num = 0; spi_num < SPI_CNT; spi_num++) {
        strcpy(temp_str, TSEP);
        snprintf(temp_str, sizeof(temp_str), "%s %3u " TSEP, temp_str, spi_num);
        snprintf(temp_str, sizeof(temp_str), "%s %1u " TSEP, temp_str, spi_get_receive_int((SpiName_t)spi_num));
        snprintf(temp_str, sizeof(temp_str), "%s %1u " TSEP, temp_str, spi_get_transmit_int((SpiName_t)spi_num));
        snprintf(temp_str, sizeof(temp_str), "%s %1u " TSEP, temp_str,
                 spi_get_receive_timeout_interrupt((SpiName_t)spi_num));
        snprintf(temp_str, sizeof(temp_str), "%s %1u " TSEP, temp_str,
                 spi_get_receive_overrun_interrupt((SpiName_t)spi_num));
        snprintf(temp_str, sizeof(temp_str), "%s %s " TSEP, temp_str, SpiInstance[spi_num].name);
        snprintf(temp_str, sizeof(temp_str), "%s" CRLF, temp_str);
        io_printf("%s", temp_str);
    }
    table_row_bottom(&dbg_o.s, cols, ARRAY_SIZE(cols));

    return res;
}

bool spi_write_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(2 == argc) {
        res = true;
        uint8_t spi_num = 0;
        uint8_t tx_array[256];
        uint32_t array_len = 0;
        res = try_str2uint8(argv[0], &spi_num);
        if(false == res) {
            LOG_ERROR(SPI, "Unable to parse SPI Number [1....8]");
        }

        if(true == res) {
            res = try_str2array(argv[1], tx_array, sizeof(tx_array), &array_len);
            if(false == res) {
                LOG_ERROR(SPI, "Unable to extract tx array %s", argv[1]);
            }
        }

        if(true == res) {
            res = spi_write((SpiName_t)spi_num, tx_array, array_len);
            if(false == res) {
                LOG_ERROR(SPI, "Unable to send SPI");
            } else {
                LOG_INFO(SPI, "OK");
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
            res = spi_read((SpiName_t)spi_num, array, array_len);
            if(false == res) {
                LOG_ERROR(SPI, "Unable to read SPI");
            } else {
                print_mem(array, array_len, false);
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
