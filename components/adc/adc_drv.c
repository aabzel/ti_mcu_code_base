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

uint32_t AdcCodes[ADC_ITEMS_CNT];
const ADCItem_t AdcItemsLUT[ADC_ITEMS_CNT] = {
    /*0*/ {ADC_COMPB_IN_DCOUPL, 30, 43},
    /*1*/ {ADC_COMPB_IN_VSS, 0, 0xff},
    /*2*/ {ADC_COMPB_IN_VDDS, 0, 0xff},
    /*3*/ {ADC_COMPB_IN_AUXIO7, 0, 0xff},
    /*4*/ {ADC_COMPB_IN_AUXIO6, 23, 36},
    /*5*/ {ADC_COMPB_IN_AUXIO5, 24, 37},
    /*6*/ {ADC_COMPB_IN_AUXIO4, 25, 38},
    /*7*/ {ADC_COMPB_IN_AUXIO3, 26, 39},
    /*8*/ {ADC_COMPB_IN_AUXIO2, 27, 40},
    /*9*/ {ADC_COMPB_IN_AUXIO1, 28, 41},
    /*10*/ {ADC_COMPB_IN_AUXIO0, 29, 42},
    /*11*/ {0xFF, 0xFF, 0xff},
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

    for(i = 0; i < ARRAY_SIZE(AdcItemsLUT); i++) {
        if(0xFF != AdcItemsLUT[i].adc_channel) {
            AUXADCSelectInput(AdcItemsLUT[i].adc_channel);
            AUXADCEnableAsync(AUXADC_REF_FIXED, AUXADC_TRIGGER_MANUAL);
            //            AUXADCEnableSync( AUXADC_REF_FIXED, AUXADC_SAMPLE_TIME_10P9_MS,  AUXADC_TRIGGER_MANUAL);
            AUXADCGenManualTrigger();
            AdcCodes[i] = AUXADCReadFifo();
            res = true;
        }
    }

    return res;
}

float adc_get_value_by_dio(uint8_t des_dio) {
    float voltage = 0.0f;
    uint8_t i = 0;
    for(i = 0; i < ARRAY_SIZE(AdcItemsLUT); i++) {
        if(des_dio == AdcItemsLUT[i].io_pin) {
            int32_t microvolts = 0;
            microvolts = AUXADCValueToMicrovolts(AUXADC_FIXED_REF_VOLTAGE_NORMAL, AdcCodes[i]);
            voltage = 0.000001f * ((float)microvolts);
            break;
        }
    }
    return voltage;
}
