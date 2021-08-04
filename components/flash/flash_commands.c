#include "flash_commands.h"

#include <inttypes.h>

#include "convert.h"
#include "data_utils.h"
#include "debug_info.h"
#include "diag_page_nums.h"
#include "diag_report.h"
#include "flash_drv.h"
#include "log.h"
#include "table_utils.h"

bool flash_scan_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(0 == argc) {
        float usage_pec = 0.0f;
        uint32_t spare = 0;
        uint32_t busy = 0;
        res = flash_scan((uint8_t*)NOR_FLASH_BASE, NOR_FLASH_SIZE, &usage_pec, &spare, &busy);
        if(res) {
            io_printf("usage: %f %%" CRLF, usage_pec);
            io_printf("spare: %u Bytes %u kBytes" CRLF, spare, spare / 1024);
            io_printf("busy : %u Bytes %u kBytes" CRLF, busy, busy / 1024);
        }

    } else {
        LOG_ERROR(SPI, "Usage: fs");
    }
    return res;
}
