#include "pwr_mux_drv.h"

#include <gpio.h>
#include <stdbool.h>
#include <stdint.h>

#ifdef HAS_ADC
#include "adc_drv.h"
#endif

#include "float_utils.h"
#include "log.h"
#include "sys_config.h"

#ifdef HAS_PARAM
#include "param_ids.h"
#endif

#ifdef HAS_FLASH_FS
#include "flash_fs.h"
#endif /*HAS_FLASH_FS*/

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
            LOG_ERROR(PWR, "Batt voltage too low %f norm: %f", vbatt_voltage, NORMAL_BATT_VOL);
        }
    } else {
        res = false;
    }
    return res;
}

bool pwr_src_init(void) {
    bool res = false;
    PwrSource_t pwr_source = PWR_SRC_UNDEF;
#ifdef HAS_FLASH_FS
    uint16_t file_len = 0;
    res = mm_get(PAR_ID_PWR_SRC, (uint8_t*)&pwr_source, sizeof(pwr_source), &file_len);
#endif /*HAS_FLASH_FS*/
    if((true == res) && (1 == file_len)) {
        LOG_INFO(PWR, "Set power source %u %s", pwr_source, pwr_source2str(pwr_source));
        res = pwr_src_set(pwr_source);
    } else {
        LOG_WARNING(PWR, "Set default power source VCC 3.3V");
        res = pwr_src_set(PWR_SRC_VCC_3V3);
        pwr_source = PWR_SRC_VCC_3V3;
        res = mm_set(PAR_ID_PWR_SRC, (uint8_t*)&pwr_source, sizeof(pwr_source));
        if ( false == res ) {
            LOG_ERROR(PWR, "Unable to set dflt PwrSrc");
        }
    }

    return res;
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
