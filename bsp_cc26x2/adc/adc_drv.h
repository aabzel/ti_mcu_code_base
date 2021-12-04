/*data sheet page 1665 1479*/
#ifndef ADC_DRV_H
#define ADC_DRV_H

#include <ioc.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct xADCItem_t {
    uint8_t adc_channel;
    uint8_t aux_pin;
    uint8_t io_pin;
    uint8_t pin;
    float scale;
} ADCItem_t;

#define ADC_ITEMS_CNT 3
extern uint32_t AdcCodes[ADC_ITEMS_CNT];
extern const ADCItem_t AdcItemsLUT[ADC_ITEMS_CNT];

extern uint16_t adcValue0;
// 12-bit ADC
bool adc_init(void);
bool adc_proc(void);
float adc_get_value_by_dio(uint8_t des_dio, bool scaled);
#endif /* ADC_DRV_H  */
