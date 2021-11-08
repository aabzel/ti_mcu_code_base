#include "uart_commands.h"

#include <inttypes.h>
#include <uart.h>

#include "base_cmd.h"
#include "convert.h"
#include "data_utils.h"
#include "diag_page_nums.h"
#include "diag_report.h"
#include "log.h"
#include "ostream.h"
#include "table_utils.h"
#include "uart_common.h"
#include "uart_drv.h"
#include "uart_string_reader.h"
#include "writer_config.h"

bool diag_page_uarts(ostream_t* stream) {
    LOG_NOTICE(SYS, "%s()", __FUNCTION__);

    static const table_col_t cols[] = {
        {16, "Name"}, {17, "Total chars"}, {13, "Lost chars"}, {11, "Strings"}, {11, "Errors"},
    };

    table_header(&(curWriterPtr->s), cols, ARRAY_SIZE(cols));
    io_printf(TABLE_LEFT "%15s " TABLE_SEPARATOR "%16" PRId64 " " TABLE_SEPARATOR "%12" PRId64 " " TABLE_SEPARATOR
                         "%10" PRId64 " " TABLE_SEPARATOR "%10" PRId64 " " TABLE_RIGHT CRLF,
              "cmd_reader", cmd_reader.total_char_count, cmd_reader.lost_char_count, cmd_reader.total_string_count,
              cmd_reader.error_count);

    io_printf(TABLE_LEFT "dbg_o " T_SEP);
    io_printf("%16d " T_SEP, dbg_o.total_char_count);
    io_printf("%12d " T_SEP, dbg_o.lost_char_count);
    io_printf("%10d " T_SEP CRLF, dbg_o.error_count);

    table_row_bottom(&(curWriterPtr->s), cols, ARRAY_SIZE(cols));

    io_printf("rx %u byte " CRLF, huart[0].rx_byte_cnt);
    io_printf("tx %u byte " CRLF, huart[0].tx_byte_cnt);
    io_printf("tx cpl %u cnt " CRLF, huart[0].tx_cpl_cnt);
    return true;
}

bool cmd_uarts(int32_t argc, char* argv[]) {
    (void)argv;
    bool res = false;
    if(0 == argc) {
        res = diag_page_uarts(DBG_UART_STREAM);
    } else if(0 < argc) {
        res = false;
        LOG_ERROR(UART, "Usage: u");
    }
    return res;
}

// us 8 byte
// us 8 hex_string
bool uart_send_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(2 == argc) {
        res = true;
        uint8_t uart_num = 0;
        uint8_t array[256] = {0};
        uint32_t array_len = 0;
        res = try_str2uint8(argv[0], &uart_num);
        if(false == res) {
            LOG_ERROR(UART, "Unable to parse UART Number [1....8]");
        }

        if(true == res) {
            res = try_str2array(argv[1], array, sizeof(array), &array_len);
            if(false == res) {
                LOG_ERROR(UART, "Unable to extract array %s", argv[1]);
            }
        }

        if(true == res) {
            res = uart_send(uart_num, array, array_len, true);
            if(false == res) {
                LOG_ERROR(UART, "Unable to send UART");
            } else {
                io_printf(CRLF);
            }
        }
    } else {
        LOG_ERROR(UART, "Usage: us instance hex_string");
        LOG_INFO(UART, "instance [1...8]");
        LOG_INFO(UART, "hex_string 0x[0...F]+");
    }
    return res;
}

bool uart_init_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(2 == argc) {
        res = true;
        uint32_t baudrate = 0;
        uint8_t uart_num = 0;
        res = try_str2uint8(argv[0], &uart_num);
        if(false == res) {
            LOG_ERROR(UART, "Unable to parse UART Number [0..1]");
        }

        if(true == res) {
            res = try_str2uint32(argv[1], &baudrate);
            if(false == res) {
                LOG_ERROR(UART, "Unable to extract baudrate %s", argv[1]);
            }
        }

        if(true == res) {
            res = uart_set_baudrate(uart_num, baudrate);
            if(false == res) {
                LOG_ERROR(UART, "Unable to set baudrate");
            }
        }
    } else {
        LOG_ERROR(UART, "Usage: us instance baudrate");
        LOG_INFO(UART, "instance [0...1]");
        LOG_INFO(UART, "baudrate ");
    }
    return res;
}

/*TODO: calculate baud_rate*/
bool uart_diag_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(0 == argc) {
        res = true;
        const table_col_t cols[] = {{5, "Num"},    {10, "baudRate"}, {9, "rx"},   {9, "tx"},   {7, "rFiCnt"},
                                    {7, "tFiCnt"}, {6, "StErr"},     {6, "rErr"}, {10, "name"}};
        uint32_t baud_rate = 0, uart_error = 0;
        uint8_t uart_num = 0;
        table_header(&(curWriterPtr->s), cols, ARRAY_SIZE(cols));
        for(uart_num = 0; uart_num < UART_COUNT; uart_num++) {
            io_printf(TSEP);
            io_printf(" %02u  " TSEP, uart_num);
            uart_error = UARTRxErrorGet(uartCC26XXHWAttrs[uart_num].baseAddr);
            baud_rate = uart_get_baudrate(uart_num);
            if(0 < baud_rate) {
                io_printf(" %7u  " TSEP, baud_rate);
            } else {
                io_printf("          " TSEP, baud_rate);
            }
            io_printf(" %07u " TSEP, huart[uart_num].rx_cnt);
            io_printf(" %07u " TSEP, huart[uart_num].tx_cnt);

            io_printf(" %4u  " TSEP, huart[uart_num].RxFifo.fifoState.count);
            io_printf(" %4u  " TSEP, huart[uart_num].TxFifo.fifoState.count);

            io_printf(" 0x%02x " TSEP, uart_error);
            io_printf(" %4u " TSEP, huart[uart_num].error_cnt);
            io_printf(" %7s  " TSEP, huart[uart_num].name);
            io_printf(CRLF);
        }
        table_row_bottom(&(curWriterPtr->s), cols, ARRAY_SIZE(cols));
    } else {
        LOG_ERROR(UART, "Usage: ud");
    }
    return res;
}

static bool uart_int_diag(bool Masked) {
    bool res = false;
    io_printf(" %s Interrupt Status" CRLF, (true == Masked) ? "Masked" : "Raw");
    const table_col_t cols[] = {
        {5, "Num"}, {12, "Is"}, {4, "Eot"}, {4, "Oe"}, {4, "Be"},  {4, "Pe"},
        {4, "Fe"},  {4, "Rt"},  {4, "Tx"},  {4, "Rx"}, {4, "CTS"}, {10, "name"},
    };
    uint32_t int_status = 0;
    uint8_t uart_num = 0;
    table_header(&(curWriterPtr->s), cols, ARRAY_SIZE(cols));
    for(uart_num = 0; uart_num < UART_COUNT; uart_num++) {
        io_printf(TSEP);
        io_printf(" %02u  " TSEP, uart_num);
        int_status = UARTIntStatus(uartCC26XXHWAttrs[uart_num].baseAddr, Masked);
        io_printf(" 0x%08x " TSEP, int_status);
        io_printf("  %u " TSEP, UART_INT_EOT == (UART_INT_EOT & int_status));
        io_printf("  %u " TSEP, UART_INT_OE == (UART_INT_OE & int_status));
        io_printf("  %u " TSEP, UART_INT_BE == (UART_INT_BE & int_status));
        io_printf("  %u " TSEP, UART_INT_PE == (UART_INT_PE & int_status));
        io_printf("  %u " TSEP, UART_INT_FE == (UART_INT_FE & int_status));
        io_printf("  %u " TSEP, UART_INT_RT == (UART_INT_RT & int_status));
        io_printf("  %u " TSEP, UART_INT_TX == (UART_INT_TX & int_status));
        io_printf("  %u " TSEP, UART_INT_RX == (UART_INT_RX & int_status));
        io_printf("  %u " TSEP, UART_INT_CTS == (UART_INT_CTS & int_status));
        io_printf(" %7s  " TSEP, huart[uart_num].name);
        io_printf(CRLF);
    }
    table_row_bottom(&(curWriterPtr->s), cols, ARRAY_SIZE(cols));
    return res;
}

bool uart_int_diag_command(int32_t argc, char* argv[]) {
    bool res = false;
    bool is_masked = false;
    if(0 == argc) {
        res = true;
    } else if(1 == argc) {
        res = try_str2bool(argv[0], &is_masked);
        if(false == res) {
            LOG_ERROR(UART, "Unable to parse type");
        }
    } else {
        LOG_ERROR(UART, "Usage: uid is_masked");
    }

    if(res) {
        res = uart_int_diag(is_masked); // RIS
    }
    return res;
}

bool uart_deinit_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(1 == argc) {
        uint8_t uart_num = 0;
        res = try_str2uint8(argv[0], &uart_num);
        if(false == res) {
            LOG_ERROR(UART, "Unable to parse UART num [0..1]");
        }
        if(res) {
            res = uart_deinit(uart_num);
            if(res) {
                LOG_INFO(UART, "Ok");
            } else {
                LOG_ERROR(UART, "Error");
            }
        }
    } else {
        LOG_ERROR(UART, "Usage: udi uart_num");
    }
    return res;
}

/*us 1 0 1*/
bool uart_forward_command(int32_t argc, char* argv[]) {
    bool res = false;
    bool status = false;
    uint8_t uart_num1 = 0;
    uint8_t uart_num2 = 0;
    if(1 <= argc) {
        res = try_str2uint8(argv[0], &uart_num1);
        if(false == res) {
            LOG_ERROR(UART, "Unable to parse uart num1 %s", argv[0]);
        }
    }
    if(2 <= argc) {
        res = try_str2uint8(argv[1], &uart_num2);
        if(false == res) {
            LOG_ERROR(UART, "Unable to parse uart num1 %s", argv[1]);
        }
    }
    if(3 <= argc) {
        res = try_str2bool(argv[2], &status);
        if(false == res) {
            LOG_ERROR(UART, "Unable to parse status %s", argv[2]);
        }
    }

    if(3 < argc) {
        LOG_ERROR(UART, "Usage: uf uart_num1 uart_nu2");
    }

    if(res) {
        huart[uart_num1].is_uart_fwd[uart_num2] = status;
    }
    return res;
}
