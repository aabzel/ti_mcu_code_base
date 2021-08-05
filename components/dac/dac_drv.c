#include "dac_drv.h"

#include <aux_dac.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
/*
./simplelink_cc13x2_26x2_sdk_5_20_00_52/source/ti/devices/cc13x2_cc26x2/driverlib/aux_dac.c
./simplelink_cc13x2_26x2_sdk_5_20_00_52/source/ti/devices/cc13x2_cc26x2/driverlib/aux_dac.h

 * */

uint8_t io2comp(uint8_t io_pin) {
    uint8_t mux_code = 0;
    switch(io_pin) {
    case 19:
        mux_code = AUXDAC_COMPA_IN_AUXIO19;
        break;
    case 20:
        mux_code = AUXDAC_COMPA_IN_AUXIO20;
        break;
    case 21:
        mux_code = AUXDAC_COMPA_IN_AUXIO21;
        break;
    case 22:
        mux_code = AUXDAC_COMPA_IN_AUXIO22;
        break;
    case 23:
        mux_code = AUXDAC_COMPA_IN_AUXIO23;
        break;
    case 24:
        mux_code = AUXDAC_COMPA_IN_AUXIO24;
        break;
    case 25:
        mux_code = AUXDAC_COMPA_IN_AUXIO25;
        break;
    case 26:
        mux_code = AUXDAC_COMPA_IN_AUXIO26;
        break;
    default:
        break;
    }
    return mux_code;
}

bool dac_init(void) {
    AUXDACDisable();
    AUXDACSetVref(AUXDAC_VREF_SEL_VDDS);
    AUXDACSetSampleClock(128);
    AUXDACSetCode(0);
    AUXDACEnable(AUXDAC_COMPA_IN_AUXIO19);
    return true;
}

bool dac_proc(void) { return false; }
bool dac_set_value_by_dio(uint8_t dio, float voltage) { return false; }
