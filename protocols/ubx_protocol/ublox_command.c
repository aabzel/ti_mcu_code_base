#include "ublox_command.h"

#include <stdbool.h>
#include <stdio.h>

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
#ifdef HAS_DEBUG
    io_printf("sync cnt: %u" CRLF, UbloxPorotocol.sync_cnt);
    io_printf("min len: %u" CRLF, UbloxPorotocol.min_len);
    io_printf("max len: %u" CRLF, UbloxPorotocol.max_len);
#endif
    uint8_t i = 0;

    table_col_t cols[] = {{7, "class"}, {8, "rx_cnt"}};
    table_header(&dbg_o.s, cols, ARRAY_SIZE(cols));
    for(i = 0; i < ARRAY_SIZE(tableRxClass); i++) {
        if(tableRxClass[i].class_val) {
            io_printf(TSEP "  0x%02x " TSEP, tableRxClass[i].class_val);
            io_printf(" %u ", tableRxClass[i].cnt);
            io_printf(CRLF);
        }
    }
    table_row_bottom(&dbg_o.s, cols, ARRAY_SIZE(cols));
    return true;
}

static char* ubx_print_key_val(uint16_t i) {
    static char buff[20] = "";
    if(i < UBX_KEY_CNT) {
        if(UBX_U2 == keyValTable[i].type) {
            snprintf(buff, sizeof(buff), " %u ", keyValTable[i].u_value.u16);
        }
    }
    return buff;
}

static bool ubx_key_val_diag(void) {
    uint8_t i = 0;
    table_col_t cols[] = {{7, "key"}, {8, "val"}};
    table_header(&dbg_o.s, cols, ARRAY_SIZE(cols));
    for(i = 0; i < ARRAY_SIZE(keyValTable); i++) {
        if(keyValTable[i].key_id) {
            io_printf(TSEP "  0x%08x " TSEP, keyValTable[i].key_id);
            io_printf(" %s ", ubx_print_key_val(i));
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

bool ubx_print_key_val_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(0 == argc) {
        res = ubx_key_val_diag();
    } else {
        LOG_ERROR(SYS, "Usage: ubk");
    }
    return res;
}

bool ubx_get_key_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(1 == argc) {
        res = true;
        uint32_t key_id = 0;
        if(true == res) {
            res = try_str2uint32(argv[0], &key_id);
            if(false == res) {
                LOG_ERROR(UART, "Unable to extract Class %s", argv[0]);
            }
        }

        if(true == res) {
            uint8_t payload[10] = {0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x21, 0x30};
            memcpy(&payload[4], &key_id, 4);
            res = ubx_send_message(0x06, 0x8b, payload, 8);
            if(false == res) {
                LOG_ERROR(SYS, "Unable to send to Ublox");
            }
        }

    } else {
        LOG_ERROR(SYS, "Usage: ubg key_id");
    }
    return res;
}

bool ubx_send_command(int32_t argc, char* argv[]) {
    bool res = false;
    uint8_t class_num = 0;
    uint8_t id = 0;
    uint32_t payload_len = 0U;
    uint8_t payload[100] = {0};
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
        } else {
            LOG_INFO(SYS, "Send OK");
        }
    } else {
        LOG_ERROR(SYS, "Usage: ubs class id payload");
    }

    return res;
}

static bool ubx_nav(void) {
    io_printf("%u ", NavInfo.latitude);
    io_printf("%u" CRLF, NavInfo.longitude);
    io_printf("hmsl: %u mm %u m" CRLF, NavInfo.hmsl, NavInfo.hmsl / 1000);
    io_printf("h_acc: %u %f mm" CRLF, NavInfo.h_acc, 0.1f * ((double)NavInfo.h_acc));
    io_printf("v_acc: %u %f mm" CRLF, NavInfo.v_acc, 0.1f * ((double)NavInfo.v_acc));
    io_printf("roll %d %f deg" CRLF, NavInfo.roll, 1e-5 * ((double)NavInfo.roll));
    io_printf("pitch %d %f deg" CRLF, NavInfo.pitch, 1e-5 * ((double)NavInfo.pitch));
    io_printf("heading %d %f deg" CRLF, NavInfo.heading, 1e-5 * ((double)NavInfo.heading));

    io_printf("acc_roll %d %f deg" CRLF, NavInfo.acc_roll, 1e-5 * ((double)NavInfo.acc_roll));
    io_printf("acc_pitch %d %f deg" CRLF, NavInfo.acc_pitch, 1e-5 * ((double)NavInfo.acc_pitch));
    io_printf("acc_heading %d %f deg" CRLF, NavInfo.acc_heading, 1e-5 * ((double)NavInfo.acc_heading));

    return true;
}

bool ubx_nav_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(0 == argc) {
        res = ubx_nav();
    } else {
        LOG_ERROR(SYS, "Usage: ubn");
    }
    return res;
}
