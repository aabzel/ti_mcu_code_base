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
    /* adc_channel   aux_pin   io_pin   pin*/
    /*10*/ {ADC_COMPB_IN_AUXIO7, 26, 23, 36},
    /*9*/ {ADC_COMPB_IN_AUXIO6, 25, 24, 37},
    /*8*/ {ADC_COMPB_IN_AUXIO5, 24, 25, 38},
    /*7*/ {ADC_COMPB_IN_AUXIO4, 23, 26, 40},
    /*6*/ {ADC_COMPB_IN_AUXIO3, 22, 27, 40},
    /*5*/ {ADC_COMPB_IN_AUXIO2, 21, 28, 41},
    /*4*/ {ADC_COMPB_IN_AUXIO1, 20, 29, 42},
    /*3*/ {ADC_COMPB_IN_AUXIO0, 19, 30, 43},
    /*0*/ {ADC_COMPB_IN_DCOUPL, 0, 0, 0},
    /*1*/ {ADC_COMPB_IN_VSS, 0, 0, 0},
    /*2*/ {ADC_COMPB_IN_VDDS, 0, 0, 0},
    /*11*/ {0xFF, 0xff, 0xFF, 0xff},
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
            //            AUXADCEnableSync( AUXADC_REF_FIXED, AUXADC_SAMPLE_TIME_10P9_MS,  AUXADC_TRIGGER_MANUAL);
            AUXADCGenManualTrigger();
            AdcCodes[i] = AUXADCReadFifo();
            //            uint8_t j;
            //          for(j=0;i<4;i++){
            //        }
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
