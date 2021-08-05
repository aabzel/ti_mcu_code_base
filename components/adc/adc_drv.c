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

ADCCC26XX_Object adcCC26xxObjects[CC26X2R1_LAUNCHXL_ADCCOUNT];

const ADCCC26XX_HWAttrs adcCC26xxHWAttrs[CC26X2R1_LAUNCHXL_ADCCOUNT] = {
    {.adcDIO = CC26X2R1_LAUNCHXL_DIO23_ANALOG,
     .adcCompBInput = ADC_COMPB_IN_AUXIO7,
     .refSource = ADCCC26XX_FIXED_REFERENCE,
     .samplingDuration = ADCCC26XX_SAMPLING_DURATION_2P7_US,
     .inputScalingEnabled = true,
     .triggerSource = ADCCC26XX_TRIGGER_MANUAL,
     .returnAdjustedVal = false},
    {.adcDIO = CC26X2R1_LAUNCHXL_DIO24_ANALOG,
     .adcCompBInput = ADC_COMPB_IN_AUXIO6,
     .refSource = ADCCC26XX_FIXED_REFERENCE,
     .samplingDuration = ADCCC26XX_SAMPLING_DURATION_2P7_US,
     .inputScalingEnabled = true,
     .triggerSource = ADCCC26XX_TRIGGER_MANUAL,
     .returnAdjustedVal = false},
    {.adcDIO = CC26X2R1_LAUNCHXL_DIO25_ANALOG,
     .adcCompBInput = ADC_COMPB_IN_AUXIO5,
     .refSource = ADCCC26XX_FIXED_REFERENCE,
     .samplingDuration = ADCCC26XX_SAMPLING_DURATION_2P7_US,
     .inputScalingEnabled = true,
     .triggerSource = ADCCC26XX_TRIGGER_MANUAL,
     .returnAdjustedVal = false},
    {.adcDIO = CC26X2R1_LAUNCHXL_DIO26_ANALOG,
     .adcCompBInput = ADC_COMPB_IN_AUXIO4,
     .refSource = ADCCC26XX_FIXED_REFERENCE,
     .samplingDuration = ADCCC26XX_SAMPLING_DURATION_2P7_US,
     .inputScalingEnabled = true,
     .triggerSource = ADCCC26XX_TRIGGER_MANUAL,
     .returnAdjustedVal = false},
    {.adcDIO = CC26X2R1_LAUNCHXL_DIO27_ANALOG,
     .adcCompBInput = ADC_COMPB_IN_AUXIO3,
     .refSource = ADCCC26XX_FIXED_REFERENCE,
     .samplingDuration = ADCCC26XX_SAMPLING_DURATION_2P7_US,
     .inputScalingEnabled = true,
     .triggerSource = ADCCC26XX_TRIGGER_MANUAL,
     .returnAdjustedVal = false},
    {.adcDIO = CC26X2R1_LAUNCHXL_DIO28_ANALOG,
     .adcCompBInput = ADC_COMPB_IN_AUXIO2,
     .refSource = ADCCC26XX_FIXED_REFERENCE,
     .samplingDuration = ADCCC26XX_SAMPLING_DURATION_2P7_US,
     .inputScalingEnabled = true,
     .triggerSource = ADCCC26XX_TRIGGER_MANUAL,
     .returnAdjustedVal = false},
    {.adcDIO = CC26X2R1_LAUNCHXL_DIO29_ANALOG,
     .adcCompBInput = ADC_COMPB_IN_AUXIO1,
     .refSource = ADCCC26XX_FIXED_REFERENCE,
     .samplingDuration = ADCCC26XX_SAMPLING_DURATION_2P7_US,
     .inputScalingEnabled = true,
     .triggerSource = ADCCC26XX_TRIGGER_MANUAL,
     .returnAdjustedVal = false},
    {.adcDIO = CC26X2R1_LAUNCHXL_DIO30_ANALOG,
     .adcCompBInput = ADC_COMPB_IN_AUXIO0,
     .refSource = ADCCC26XX_FIXED_REFERENCE,
     .samplingDuration = ADCCC26XX_SAMPLING_DURATION_10P9_MS,
     .inputScalingEnabled = true,
     .triggerSource = ADCCC26XX_TRIGGER_MANUAL,
     .returnAdjustedVal = false},
    {.adcDIO = PIN_UNASSIGNED,
     .adcCompBInput = ADC_COMPB_IN_DCOUPL,
     .refSource = ADCCC26XX_FIXED_REFERENCE,
     .samplingDuration = ADCCC26XX_SAMPLING_DURATION_2P7_US,
     .inputScalingEnabled = true,
     .triggerSource = ADCCC26XX_TRIGGER_MANUAL,
     .returnAdjustedVal = false},
    {.adcDIO = PIN_UNASSIGNED,
     .adcCompBInput = ADC_COMPB_IN_VSS,
     .refSource = ADCCC26XX_FIXED_REFERENCE,
     .samplingDuration = ADCCC26XX_SAMPLING_DURATION_2P7_US,
     .inputScalingEnabled = true,
     .triggerSource = ADCCC26XX_TRIGGER_MANUAL,
     .returnAdjustedVal = false},
    {.adcDIO = PIN_UNASSIGNED,
     .adcCompBInput = ADC_COMPB_IN_VDDS,
     .refSource = ADCCC26XX_FIXED_REFERENCE,
     .samplingDuration = ADCCC26XX_SAMPLING_DURATION_2P7_US,
     .inputScalingEnabled = true,
     .triggerSource = ADCCC26XX_TRIGGER_MANUAL,
     .returnAdjustedVal = false}};

const ADC_Config ADC_config[CC26X2R1_LAUNCHXL_ADCCOUNT] = {
    {&ADCCC26XX_fxnTable, &adcCC26xxObjects[CC26X2R1_LAUNCHXL_ADC0], &adcCC26xxHWAttrs[CC26X2R1_LAUNCHXL_ADC0]},
    {&ADCCC26XX_fxnTable, &adcCC26xxObjects[CC26X2R1_LAUNCHXL_ADC1], &adcCC26xxHWAttrs[CC26X2R1_LAUNCHXL_ADC1]},
    {&ADCCC26XX_fxnTable, &adcCC26xxObjects[CC26X2R1_LAUNCHXL_ADC2], &adcCC26xxHWAttrs[CC26X2R1_LAUNCHXL_ADC2]},
    {&ADCCC26XX_fxnTable, &adcCC26xxObjects[CC26X2R1_LAUNCHXL_ADC3], &adcCC26xxHWAttrs[CC26X2R1_LAUNCHXL_ADC3]},
    {&ADCCC26XX_fxnTable, &adcCC26xxObjects[CC26X2R1_LAUNCHXL_ADC4], &adcCC26xxHWAttrs[CC26X2R1_LAUNCHXL_ADC4]},
    {&ADCCC26XX_fxnTable, &adcCC26xxObjects[CC26X2R1_LAUNCHXL_ADC5], &adcCC26xxHWAttrs[CC26X2R1_LAUNCHXL_ADC5]},
    {&ADCCC26XX_fxnTable, &adcCC26xxObjects[CC26X2R1_LAUNCHXL_ADC6], &adcCC26xxHWAttrs[CC26X2R1_LAUNCHXL_ADC6]},
    {&ADCCC26XX_fxnTable, &adcCC26xxObjects[CC26X2R1_LAUNCHXL_ADC7], &adcCC26xxHWAttrs[CC26X2R1_LAUNCHXL_ADC7]},
    {&ADCCC26XX_fxnTable, &adcCC26xxObjects[CC26X2R1_LAUNCHXL_ADCDCOUPL],
     &adcCC26xxHWAttrs[CC26X2R1_LAUNCHXL_ADCDCOUPL]},
    {&ADCCC26XX_fxnTable, &adcCC26xxObjects[CC26X2R1_LAUNCHXL_ADCVSS], &adcCC26xxHWAttrs[CC26X2R1_LAUNCHXL_ADCVSS]},
    {&ADCCC26XX_fxnTable, &adcCC26xxObjects[CC26X2R1_LAUNCHXL_ADCVDDS], &adcCC26xxHWAttrs[CC26X2R1_LAUNCHXL_ADCVDDS]},
};

const uint_least8_t ADC_count = CC26X2R1_LAUNCHXL_ADCCOUNT;

ADC_Handle adcHandle = NULL;
ADC_Params adcParams;
uint16_t adcValue0;

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
    bool res = false;
    AUXADCDisable();
    AUXADCFlushFifo();
    AUXADCSelectInput(ADC_COMPB_IN_VDDS);
    AUXADCEnableAsync(AUXADC_REF_FIXED, AUXADC_TRIGGER_MANUAL);
    AUXADCGenManualTrigger();
    return res;
}

bool adc_proc(void) {
    bool res = false;
    uint8_t i = 0;

    for(i = 0; i < ARRAY_SIZE(AdcItemsLUT); i++) {
        if(0xFF != AdcItemsLUT[i].adc_channel) {
            AUXADCSelectInput(AdcItemsLUT[i].adc_channel);
            AUXADCEnableAsync(AUXADC_REF_FIXED, AUXADC_TRIGGER_MANUAL);
            AUXADCGenManualTrigger();
            AdcCodes[i] = AUXADCReadFifo();
            res = true;
        }
    }

    return res;
}
