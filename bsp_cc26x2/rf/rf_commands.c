#include "rf_commands.h"

#include <inttypes.h>

#include "convert.h"
#include "data_utils.h"
#include "debug_info.h"
#include "diag_report.h"
#include "log.h"
#include "none_blocking_pause.h"
#include "rf_drv.h"
#include "table_utils.h"

bool rf_write_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(1 == argc) {
        res = true;

        uint8_t array[256];
        uint16_t array_len = 0;

        if(true == res) {
            res = try_str2array(argv[0], array, sizeof(array), &array_len);
            if(false == res) {
                LOG_ERROR(RF, "Unable to extract array %s", argv[0]);
            }
        }

        if(true == res) {
            res = rf_write(array, array_len);
            if(false == res) {
                LOG_ERROR(RF, "Unable to send RF");
            } else {
                LOG_INFO(RF, "OK");
                io_printf(CRLF);
            }
        }
    } else {
        LOG_ERROR(RF, "Usage: rfw hex_string");
        LOG_INFO(RF, "hex_string 0x[0...F]+");
    }
    return res;
}

bool rf_read_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(0 == argc) {
        res = true;
        uint8_t array[PAYLOAD_LENGTH + 5];
        uint16_t array_len = 0;

        res = rf_tune_reception();

        if(true == res) {
            res = rf_read(array, sizeof(array), &array_len);
            if(false == res) {
                LOG_ERROR(RF, "Unable to read RF");
            } else {
                print_mem(array, array_len, true);
            }
        }
    } else {
        LOG_ERROR(RF, "Usage: rfr");
    }
    return res;
}

bool rf_diag_command(int32_t argc, char* argv[]) {
    bool res = false;
    io_printf("rx cnt %u" CRLF, rf_rx_cnt);
    io_printf("ID: %u" CRLF, rfParams.nID);
    io_printf("InactivityTimeout %u us" CRLF, rfParams.nInactivityTimeout);
    io_printf("PowerUpDuration: %u us" CRLF, rfParams.nPowerUpDuration);
    io_printf("PowerUpDurationMargin: %u" CRLF, rfParams.nPowerUpDurationMargin);
    io_printf("ClientEventMask: 0x%08x" CRLF, rfParams.nClientEventMask);
    // io_printf("clkReqAccess 0x%x08" CRLF,rfHandle->state.clkReqAccess);
    io_printf("Error CallBack: 0x%08x" CRLF, rfParams.pErrCb);
    io_printf("Event CallBack: 0x%08x" CRLF, rfParams.pClientEventCb);
    io_printf("Power CallBack: 0x%08x" CRLF, rfParams.pPowerCb);
    return res;
}

bool rf_init_command(int32_t argc, char* argv[]) {
    bool res = rf_init();
    if(false == res) {
        LOG_ERROR(RF, "rf_init error");
    }
    return res;
}
