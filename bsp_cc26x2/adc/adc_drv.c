#include "adc_drv.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#ifdef DeviceFamily_CC26X2
#include <ti/drivers/ADC.h>
#include <ti/drivers/adc/ADCCC26XX.h>
#endif

#include "data_utils.h"
#include "sys_config.h"

uint32_t AdcCodes[ADC_ITEMS_CNT];
const ADCItem_t AdcItemsLUT[ADC_ITEMS_CNT] = {
    /* adc_channel              aux_pin    io_pin         pin   scale*/
    /*9*/ {ADC_COMPB_IN_AUXIO6, 25, DIO_KL30_ADC, 37, VOL_DIV_SCALE_PWR},
    /*8*/ {ADC_COMPB_IN_AUXIO5, 24, DIO_BATT_ADC, 38, VOL_DIV_SCALE_ACC},
    /*11*/ {0xFF, 0xff, 0xFF, 0xff, 999.99f},
};

bool adc_init(void) {
    AUXADCDisable();
    AUXADCFlushFifo();
    AUXADCSelectInput(ADC_COMPB_IN_VDDS);
    //  AUXADCEnableSync( AUXADC_REF_FIXED, AUXADC_SAMPLE_TIME_10P9_MS,  AUXADC_TRIGGER_MANUAL);
    AUXADCEnableAsync(AUXADC_REF_FIXED, AUXADC_TRIGGER_MANUAL);
    AUXADCGenManualTrigger();
    return true;
}

bool adc_proc(void) {
    bool res = false;
    uint8_t i = 0;
    AUXADCFlushFifo();

    for(i = 0; i < ARRAY_SIZE(AdcItemsLUT); i++) {
        if(0xFF != AdcItemsLUT[i].adc_channel) {
            AUXADCSelectInput(AdcItemsLUT[i].adc_channel);
            AUXADCEnableAsync(AUXADC_REF_FIXED, AUXADC_TRIGGER_MANUAL);
            /*AUXADCEnableSync( AUXADC_REF_FIXED, AUXADC_SAMPLE_TIME_10P9_MS,  AUXADC_TRIGGER_MANUAL);*/
            AUXADCGenManualTrigger();
            AdcCodes[i] = AUXADCReadFifo();
#if 0
            uint8_t j;
            for(j=0;i<4;i++){
            }
#endif
            res = true;
        }
    }

    return res;
}

float adc_get_value_by_dio(uint8_t des_dio, bool scaled) {
    float voltage = 0.0f;
    uint8_t i = 0;
    for(i = 0; i < ARRAY_SIZE(AdcItemsLUT); i++) {
        if(des_dio == AdcItemsLUT[i].io_pin) {
            int32_t microvolts = 0;
            microvolts = AUXADCValueToMicrovolts(AUXADC_FIXED_REF_VOLTAGE_NORMAL, AdcCodes[i]);
            voltage = 0.000001f * ((float)microvolts);
            if(true == scaled) {
                voltage *= AdcItemsLUT[i].scale;
            }
            break;
        }
    }
    return voltage;
}
