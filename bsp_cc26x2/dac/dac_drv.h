/*data sheet page*/
#ifndef DAC_DRV_H
#define DAC_DRV_H

#include <ioc.h>
#include <stdbool.h>
#include <stdint.h>

uint8_t io2comp(uint8_t io_pin);
bool dac_init(void);
bool dac_proc(void);
bool dac_set_value_by_dio(uint8_t dio, float voltage);

#endif /* DAC_DRV_H  */
