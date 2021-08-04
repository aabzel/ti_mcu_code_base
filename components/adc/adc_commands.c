#include "adc_commands.h"

#include <inttypes.h>

#include "adc_drv.h"
#include "convert.h"
#include "data_utils.h"
#include "log.h"
#include "io_utils.h"
#include "table_utils.h"

bool adc_diag_command(int32_t argc, char* argv[]) {
    bool res = false;
    if (0==argc) {
        res = true;
        io_printf("0: %u"CRLF,adcValue0);
    } else {
        LOG_ERROR(ADC, "Usage: ain");
    }
    return res;
}
