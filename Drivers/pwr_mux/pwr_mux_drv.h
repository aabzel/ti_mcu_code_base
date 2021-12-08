
#ifndef PWR_MUX_DRV_H
#define PWR_MUX_DRV_H

#include <stdbool.h>
#include <stdint.h>

#define NORMAL_BATT_VOL 4.3f

typedef enum ePwrSource_t { PWR_SRC_VCC_3V3 = 0, PWR_SRC_3V0_BATT = 1, PWR_SRC_UNDEF = 2 } PwrSource_t;

bool pwr_src_init(void);
bool pwr_src_set(PwrSource_t source);
PwrSource_t pwr_src_get(void);

#endif /* PWR_MUX_DRV_H  */
