/*data sheet page 1665*/
#ifndef ADC_DRV_H
#define ADC_DRV_H

#include <stdbool.h>
#include <stdint.h>
#include <ioc.h>

//#define CONFIG_ADC_0 0

#define CC26X2R1_LAUNCHXL_DIO23_ANALOG          IOID_23
#define CC26X2R1_LAUNCHXL_DIO24_ANALOG          IOID_24
#define CC26X2R1_LAUNCHXL_DIO25_ANALOG          IOID_25
#define CC26X2R1_LAUNCHXL_DIO26_ANALOG          IOID_26
#define CC26X2R1_LAUNCHXL_DIO27_ANALOG          IOID_27
#define CC26X2R1_LAUNCHXL_DIO28_ANALOG          IOID_28
#define CC26X2R1_LAUNCHXL_DIO29_ANALOG          IOID_29
#define CC26X2R1_LAUNCHXL_DIO30_ANALOG          IOID_30

#define ADC_ITEMS_CNT 12
/*!
 *  @def    CC26X2R1_LAUNCHXL_ADCName
 *  @brief  Enum of ADCs
 */
typedef enum CC26X2R1_LAUNCHXL_ADCName {
    CC26X2R1_LAUNCHXL_ADC0 = 0,
    CC26X2R1_LAUNCHXL_ADC1,
    CC26X2R1_LAUNCHXL_ADC2,
    CC26X2R1_LAUNCHXL_ADC3,
    CC26X2R1_LAUNCHXL_ADC4,
    CC26X2R1_LAUNCHXL_ADC5,
    CC26X2R1_LAUNCHXL_ADC6,
    CC26X2R1_LAUNCHXL_ADC7,
    CC26X2R1_LAUNCHXL_ADCDCOUPL,
    CC26X2R1_LAUNCHXL_ADCVSS,
    CC26X2R1_LAUNCHXL_ADCVDDS,

    CC26X2R1_LAUNCHXL_ADCCOUNT
} CC26X2R1_LAUNCHXL_ADCName;


typedef struct xADCItem_t{
    uint8_t adc_channel;
    uint8_t io_pin;
    uint8_t pin;
}ADCItem_t;

extern uint32_t AdcCodes[ADC_ITEMS_CNT];
extern const ADCItem_t AdcItemsLUT[ADC_ITEMS_CNT];

extern uint16_t adcValue0;
//12-bit ADC
bool adc_init(void);
bool adc_proc(void);

#endif /* ADC_DRV_H  */
