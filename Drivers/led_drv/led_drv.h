#ifndef LED_DRV_H
#define LED_DRV_H

#include <stdbool.h>
#include <stdint.h>

#ifdef HAS_BOOTLOADER
#define LED_GREEN_PERIOD_MS 200
#else
#define LED_GREEN_PERIOD_MS 1000
#endif

#define LED_CNT 2
#define LED_INDEX_GREEN 0
#define LED_INDEX_RED 1

#define LED_DUTY 50 /*percent*/
#define LED_PHASE 0

#define LED_RED_PERIOD_MS 250
#define LED_RED_DUTY 50 /*percent*/
#define LED_RED_PHASE 1

#define LED_POLL_PERIOD_US 500

typedef enum uLedMode_t{
    LED_MODE_NONE,
    LED_MODE_PWM,
    LED_MODE_ON,
    LED_MODE_OFF,
    LED_MODE_BLINK,
}LedMode_t;

typedef struct xLed_t {
    uint32_t on_time_ms;
    uint32_t period_ms;
    uint32_t duration_ms;
    uint32_t phase_ms;
    uint8_t duty;
    uint8_t dio_num;
    LedMode_t mode;
} Led_t;

extern Led_t Led[2];

bool led_init(void);
bool led_blink(Led_t *inLed, uint32_t duration_ms);
bool led_on(Led_t *inLed);
bool led_off(Led_t *inLed);
bool proc_leds(void);

#endif /* LED_DRV_H  */
