#include "pwr_mux_diag.h"

#include "pwr_mux_drv.h"

const char* pwr_source2str(PwrSource_t pwr_source) {
    const char* name = "undef";
    switch(pwr_source) {
    case PWR_SRC_VCC_3V3:
        name = "Vcc_3.3V";
        break;
    case PWR_SRC_3V0_BATT:
        name = "Batt_3.0V";
        break;
    default:
        name = "Undef";
        break;
    }
    return name;
}


