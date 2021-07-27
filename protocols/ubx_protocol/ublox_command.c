#include "ublox_command.h"

#include <stdbool.h>

#include "convert.h"
#include "data_utils.h"
#include "io_utils.h"
#include "log.h"
#include "table_utils.h"
#include "ublox_driver.h"
#include "ubx_protocol.h"
#include "writer_generic.h"

static bool ubx_diag(void) {
    io_printf("rx pkt cnt: %u" CRLF, UbloxPorotocol.rx_pkt_cnt);
    io_printf("crc cnt   : %u" CRLF, UbloxPorotocol.crc_err_cnt);
    io_printf("rx state  : %u" CRLF, UbloxPorotocol.rx_state);
    io_printf("ack cnt   : %u" CRLF, UbloxPorotocol.ack_cnt);
    uint8_t i = 0;

    table_col_t cols[] = {{7, "class"}, {8, "rx_cnt"}};
    table_header(&dbg_o.s, cols, ARRAY_SIZE(cols));
    for(i = 0; i < ARRAY_SIZE(tableRxClass); i++) {
        if(tableRxClass[i].class_val) {
            io_printf(TSEP "  0x%02x " TSEP, tableRxClass[i].class_val);
            io_printf(" %u " TSEP, tableRxClass[i].cnt);
            io_printf(CRLF);
        }
    }
    table_row_bottom(&dbg_o.s, cols, ARRAY_SIZE(cols));
    return true;
}

bool ubx_diag_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(0 == argc) {
        res = ubx_diag();
    } else {
        LOG_ERROR(SYS, "Usage: ubd");
    }
    return res;
}

bool ubx_send_command(int32_t argc, char* argv[]) {
    bool res = false;
    uint8_t class_num = 0;
    uint8_t id = 0;
    uint16_t payload_len = 0U;
    uint8_t payload[100];
    if(2 <= argc) {
        res = true;
        payload_len = 0U;
        if(true == res) {
            res = try_str2uint8(argv[0], &class_num);
            if(false == res) {
                LOG_ERROR(UART, "Unable to extract Class %s", argv[0]);
            }
        }
        if(true == res) {
            res = try_str2uint8(argv[1], &id);
            if(false == res) {
                LOG_ERROR(UART, "Unable to extract ID %s", argv[1]);
            }
        }
    }
    if(3 == argc) {
        res = true;
        if(true == res) {
            res = try_str2array(argv[2], payload, sizeof(payload), &payload_len);
            if(false == res) {
                LOG_ERROR(UART, "Unable to extract array %s", argv[2]);
            }
        }
    }

    if(true == res) {
        res = ubx_send_message(class_num, id, payload, payload_len);
        if(false == res) {
            LOG_ERROR(SYS, "Unable to send to Ublox");
        }
    } else {
        LOG_ERROR(SYS, "Usage: ubs class id payload");
    }

    return res;
}
