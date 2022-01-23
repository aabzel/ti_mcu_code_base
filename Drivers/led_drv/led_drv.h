#ifndef LED_DRIVER_H
#define LED_DRIVER_H

#include <stdbool.h>
#include <stdint.h>

#include "sys_config.h"

#ifdef HAS_BOOTLOADER
#define LED_GREEN_PERIOD_MS 200
#else
#define LED_GREEN_PERIOD_MS 1000
#endif

#define LED_POLL_PERIOD_US 500

#define LED_DUTY 50 /*percent*/
#define LED_PHASE 0

#ifdef DIO_LED_RED
#define LED_RED_PERIOD_MS 250
#define LED_RED_DUTY 50 /*percent*/
#define LED_RED_PHASE 1
#endif


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

extern Led_t Led[LED_COUNT];
#ifdef HAS_FREE_RTOS
void led_create_task(void);
#endif
bool led_init(void);
bool led_blink(Led_t *inLed, uint32_t duration_ms);
bool led_on(Led_t *inLed);
bool led_off(Led_t *inLed);
bool proc_leds(void);

#endif /* LED_DRIVER_H  */
