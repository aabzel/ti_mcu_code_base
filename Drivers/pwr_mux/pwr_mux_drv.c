#include "pwr_mux_drv.h"

#include <gpio.h>
#include <stdbool.h>
#include <stdint.h>

#ifdef HAS_ADC
#include "adc_drv.h"
#endif /*HAS_ADC*/
#include "float_utils.h"
#ifdef HAS_LOG
#include "log.h"
#endif
#include "sys_config.h"

#ifdef HAS_PARAM
#include "param_ids.h"
#endif /*HAS_PARAM*/
#include "pwr_mux_diag.h"

#ifdef HAS_FLASH_FS
#include "flash_fs.h"
#endif /*HAS_FLASH_FS*/

bool pwr_src_set(PwrSource_t source) {
    bool res = true;
    if(PWR_SRC_VCC_3V3 == source) {
        GPIO_writeDio(DIO_PWR_MUX_CTRL, 0);
    } else if(PWR_SRC_3V0_BATT == source) {
        float vbatt_voltage = 0.0f;
#ifdef HAS_ADC
        vbatt_voltage = adc_get_value_by_dio(DIO_BATT_ADC, true);
#endif
        if(true == is_float_equal_absolute(vbatt_voltage, NORMAL_BATT_VOL, 0.5f)) {
            GPIO_writeDio(DIO_PWR_MUX_CTRL, 1);
        } else {
#ifdef HAS_LOG
            LOG_ERROR(LG_PWR, "Batt voltage too low %f norm: %f", vbatt_voltage, NORMAL_BATT_VOL);
#endif
        }
    } else {
        res = false;
    }
    return res;
}

bool pwr_src_init(void) {
    bool res = false;
    bool out_res = true;
    PwrSource_t pwr_source = PWR_SRC_VCC_3V3;
#ifdef HAS_PARAM
    LOAD_PARAM(LG_PWR, PAR_ID_PWR_SRC, pwr_source, 1, "PwrSource", PWR_SRC_VCC_3V3, pwr_source2str) ;
#endif /*HAS_FLASH_FS*/
    res = pwr_src_set(pwr_source);
    return out_res;
}

PwrSource_t pwr_src_get(void) {
    PwrSource_t cur_prw_src = PWR_SRC_UNDEF;
    uint32_t val = 0;
    val = GPIO_readDio(DIO_PWR_MUX_CTRL);
    switch(val) {
    case 0:
        cur_prw_src = PWR_SRC_VCC_3V3;
        break;
    case 1:
        cur_prw_src = PWR_SRC_3V0_BATT;
        break;
    default:
        cur_prw_src = PWR_SRC_UNDEF;
        break;
    }
    return cur_prw_src;
}
