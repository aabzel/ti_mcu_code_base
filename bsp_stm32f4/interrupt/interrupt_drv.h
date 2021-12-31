#ifndef INTERRUPT_DRV_H
#define INTERRUPT_DRV_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "stm32f4xx_hal.h"

bool interrupt_init(void);

#ifdef __cplusplus
}
#endif

#endif /* INTERRUPT_DRV_H  */
