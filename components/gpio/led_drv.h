#ifndef LED_DRV_H
#define LED_DRV_H

#include <stdbool.h>
#include <stdint.h>

#define LED_PERIOD_MS 1000
#define LED_DUTY 50 /*percent*/
#define LED_PHASE 0

typedef struct xLed_t {
    uint32_t period_ms;
    uint32_t phase_ms;
    uint8_t duty;
} Led_t;

extern Led_t greenLed;

bool led_init(void);
bool proc_led(void);

#endif /* LED_DRV_H  */
