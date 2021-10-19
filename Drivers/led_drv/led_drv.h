#ifndef LED_DRV_H
#define LED_DRV_H

#include <stdbool.h>
#include <stdint.h>

#ifdef HAS_BOOTLOADER
#define LED_GREEN_PERIOD_MS 200
#else
#define LED_GREEN_PERIOD_MS 1000
#endif

#define LED_DUTY 50 /*percent*/
#define LED_PHASE 0

#define LED_RED_PERIOD_MS 250
#define LED_RED_DUTY 50 /*percent*/
#define LED_RED_PHASE 1

#define LED_POLL_PERIOD_US 500

typedef struct xLed_t {
    uint32_t period_ms;
    uint32_t phase_ms;
    uint8_t duty;
} Led_t;

extern Led_t greenLed;
extern Led_t redLed;

bool led_init(void);
bool proc_led(void);

#endif /* LED_DRV_H  */
