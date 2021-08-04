#include "adc_drv.h"

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#ifdef DeviceFamily_CC26X2
#include <ti/drivers/ADC.h>
#include <ti/drivers/adc/ADCCC26XX.h>
#endif

ADCCC26XX_Object adcCC26xxObjects[CC26X2R1_LAUNCHXL_ADCCOUNT];

const ADCCC26XX_HWAttrs adcCC26xxHWAttrs[CC26X2R1_LAUNCHXL_ADCCOUNT] = {
    {
        .adcDIO              = CC26X2R1_LAUNCHXL_DIO23_ANALOG,
        .adcCompBInput       = ADC_COMPB_IN_AUXIO7,
        .refSource           = ADCCC26XX_FIXED_REFERENCE,
        .samplingDuration    = ADCCC26XX_SAMPLING_DURATION_2P7_US,
        .inputScalingEnabled = true,
        .triggerSource       = ADCCC26XX_TRIGGER_MANUAL,
        .returnAdjustedVal   = false
    },
    {
        .adcDIO              = CC26X2R1_LAUNCHXL_DIO24_ANALOG,
        .adcCompBInput       = ADC_COMPB_IN_AUXIO6,
        .refSource           = ADCCC26XX_FIXED_REFERENCE,
        .samplingDuration    = ADCCC26XX_SAMPLING_DURATION_2P7_US,
        .inputScalingEnabled = true,
        .triggerSource       = ADCCC26XX_TRIGGER_MANUAL,
        .returnAdjustedVal   = false
    },
    {
        .adcDIO              = CC26X2R1_LAUNCHXL_DIO25_ANALOG,
        .adcCompBInput       = ADC_COMPB_IN_AUXIO5,
        .refSource           = ADCCC26XX_FIXED_REFERENCE,
        .samplingDuration    = ADCCC26XX_SAMPLING_DURATION_2P7_US,
        .inputScalingEnabled = true,
        .triggerSource       = ADCCC26XX_TRIGGER_MANUAL,
        .returnAdjustedVal   = false
    },
    {
        .adcDIO              = CC26X2R1_LAUNCHXL_DIO26_ANALOG,
        .adcCompBInput       = ADC_COMPB_IN_AUXIO4,
        .refSource           = ADCCC26XX_FIXED_REFERENCE,
        .samplingDuration    = ADCCC26XX_SAMPLING_DURATION_2P7_US,
        .inputScalingEnabled = true,
        .triggerSource       = ADCCC26XX_TRIGGER_MANUAL,
        .returnAdjustedVal   = false
    },
    {
        .adcDIO              = CC26X2R1_LAUNCHXL_DIO27_ANALOG,
        .adcCompBInput       = ADC_COMPB_IN_AUXIO3,
        .refSource           = ADCCC26XX_FIXED_REFERENCE,
        .samplingDuration    = ADCCC26XX_SAMPLING_DURATION_2P7_US,
        .inputScalingEnabled = true,
        .triggerSource       = ADCCC26XX_TRIGGER_MANUAL,
        .returnAdjustedVal   = false
    },
    {
        .adcDIO              = CC26X2R1_LAUNCHXL_DIO28_ANALOG,
        .adcCompBInput       = ADC_COMPB_IN_AUXIO2,
        .refSource           = ADCCC26XX_FIXED_REFERENCE,
        .samplingDuration    = ADCCC26XX_SAMPLING_DURATION_2P7_US,
        .inputScalingEnabled = true,
        .triggerSource       = ADCCC26XX_TRIGGER_MANUAL,
        .returnAdjustedVal   = false
    },
    {
        .adcDIO              = CC26X2R1_LAUNCHXL_DIO29_ANALOG,
        .adcCompBInput       = ADC_COMPB_IN_AUXIO1,
        .refSource           = ADCCC26XX_FIXED_REFERENCE,
        .samplingDuration    = ADCCC26XX_SAMPLING_DURATION_2P7_US,
        .inputScalingEnabled = true,
        .triggerSource       = ADCCC26XX_TRIGGER_MANUAL,
        .returnAdjustedVal   = false
    },
    {
        .adcDIO              = CC26X2R1_LAUNCHXL_DIO30_ANALOG,
        .adcCompBInput       = ADC_COMPB_IN_AUXIO0,
        .refSource           = ADCCC26XX_FIXED_REFERENCE,
        .samplingDuration    = ADCCC26XX_SAMPLING_DURATION_10P9_MS,
        .inputScalingEnabled = true,
        .triggerSource       = ADCCC26XX_TRIGGER_MANUAL,
        .returnAdjustedVal   = false
    },
    {
        .adcDIO              = PIN_UNASSIGNED,
        .adcCompBInput       = ADC_COMPB_IN_DCOUPL,
        .refSource           = ADCCC26XX_FIXED_REFERENCE,
        .samplingDuration    = ADCCC26XX_SAMPLING_DURATION_2P7_US,
        .inputScalingEnabled = true,
        .triggerSource       = ADCCC26XX_TRIGGER_MANUAL,
        .returnAdjustedVal   = false
    },
    {
        .adcDIO              = PIN_UNASSIGNED,
        .adcCompBInput       = ADC_COMPB_IN_VSS,
        .refSource           = ADCCC26XX_FIXED_REFERENCE,
        .samplingDuration    = ADCCC26XX_SAMPLING_DURATION_2P7_US,
        .inputScalingEnabled = true,
        .triggerSource       = ADCCC26XX_TRIGGER_MANUAL,
        .returnAdjustedVal   = false
    },
    {
        .adcDIO              = PIN_UNASSIGNED,
        .adcCompBInput       = ADC_COMPB_IN_VDDS,
        .refSource           = ADCCC26XX_FIXED_REFERENCE,
        .samplingDuration    = ADCCC26XX_SAMPLING_DURATION_2P7_US,
        .inputScalingEnabled = true,
        .triggerSource       = ADCCC26XX_TRIGGER_MANUAL,
        .returnAdjustedVal   = false
    }
};

const ADC_Config ADC_config[CC26X2R1_LAUNCHXL_ADCCOUNT] = {
    {&ADCCC26XX_fxnTable, &adcCC26xxObjects[CC26X2R1_LAUNCHXL_ADC0], &adcCC26xxHWAttrs[CC26X2R1_LAUNCHXL_ADC0]},
    {&ADCCC26XX_fxnTable, &adcCC26xxObjects[CC26X2R1_LAUNCHXL_ADC1], &adcCC26xxHWAttrs[CC26X2R1_LAUNCHXL_ADC1]},
    {&ADCCC26XX_fxnTable, &adcCC26xxObjects[CC26X2R1_LAUNCHXL_ADC2], &adcCC26xxHWAttrs[CC26X2R1_LAUNCHXL_ADC2]},
    {&ADCCC26XX_fxnTable, &adcCC26xxObjects[CC26X2R1_LAUNCHXL_ADC3], &adcCC26xxHWAttrs[CC26X2R1_LAUNCHXL_ADC3]},
    {&ADCCC26XX_fxnTable, &adcCC26xxObjects[CC26X2R1_LAUNCHXL_ADC4], &adcCC26xxHWAttrs[CC26X2R1_LAUNCHXL_ADC4]},
    {&ADCCC26XX_fxnTable, &adcCC26xxObjects[CC26X2R1_LAUNCHXL_ADC5], &adcCC26xxHWAttrs[CC26X2R1_LAUNCHXL_ADC5]},
    {&ADCCC26XX_fxnTable, &adcCC26xxObjects[CC26X2R1_LAUNCHXL_ADC6], &adcCC26xxHWAttrs[CC26X2R1_LAUNCHXL_ADC6]},
    {&ADCCC26XX_fxnTable, &adcCC26xxObjects[CC26X2R1_LAUNCHXL_ADC7], &adcCC26xxHWAttrs[CC26X2R1_LAUNCHXL_ADC7]},
    {&ADCCC26XX_fxnTable, &adcCC26xxObjects[CC26X2R1_LAUNCHXL_ADCDCOUPL], &adcCC26xxHWAttrs[CC26X2R1_LAUNCHXL_ADCDCOUPL]},
    {&ADCCC26XX_fxnTable, &adcCC26xxObjects[CC26X2R1_LAUNCHXL_ADCVSS], &adcCC26xxHWAttrs[CC26X2R1_LAUNCHXL_ADCVSS]},
    {&ADCCC26XX_fxnTable, &adcCC26xxObjects[CC26X2R1_LAUNCHXL_ADCVDDS], &adcCC26xxHWAttrs[CC26X2R1_LAUNCHXL_ADCVDDS]},
};

const uint_least8_t ADC_count = CC26X2R1_LAUNCHXL_ADCCOUNT;

ADC_Handle   adcHandle=NULL;
ADC_Params   adcParams;
uint16_t adcValue0;

const uint32_t AdcChannelLUT[11]={
  ADC_COMPB_IN_DCOUPL,
 ADC_COMPB_IN_VSS,
  ADC_COMPB_IN_VDDS,
  ADC_COMPB_IN_AUXIO7,
 ADC_COMPB_IN_AUXIO6,
ADC_COMPB_IN_AUXIO5,
ADC_COMPB_IN_AUXIO4,
ADC_COMPB_IN_AUXIO3,
ADC_COMPB_IN_AUXIO2,
ADC_COMPB_IN_AUXIO1,
 ADC_COMPB_IN_AUXIO0,
};

bool adc_init(void) {
    bool res = false;
    AUXADCDisable();
    AUXADCFlushFifo();
    AUXADCSelectInput(ADC_COMPB_IN_VDDS );
    AUXADCEnableAsync(AUXADC_REF_FIXED, AUXADC_TRIGGER_MANUAL);
    AUXADCGenManualTrigger();
    //ADC_init();

//    ADC_Params_init(&adcParams);
  //  adcParams.isProtected = true;
  // adcHandle = ADC_open(0, &adcParams);
  //  if (NULL == adcHandle) {
  //      res = false;
  //  }else{
  //      res = true;
  //  }
    return res;
}

bool adc_proc(void){
    bool res = false;
    int_fast16_t ret=0;
   // ret = ADC_convert(adcHandle, &adcValue0);
    if (ADC_STATUS_SUCCESS==ret ) {
        res = true;
    }
    return res;
}
