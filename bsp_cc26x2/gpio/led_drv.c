#include "led_drv.h"

#include <gpio.h>
#include <stddef.h>
#include <stdint.h>
#include <ti/drivers/GPIO.h>

#include "clocks.h"
#include "gpio_drv.h"
#ifdef HAS_HEALTH_MONITOR
#include "health_monitor.h"
#endif /*HAS_HEALTH_MONITOR*/

Led_t greenLed;
Led_t redLed;

bool led_init(void) {
    greenLed.period_ms = LED_PERIOD_MS;
    greenLed.duty = LED_DUTY;
    greenLed.phase_ms = LED_PHASE;

    redLed.period_ms = LED_RED_PERIOD_MS;
    redLed.duty = LED_RED_DUTY;
    redLed.phase_ms = LED_RED_PHASE;

    GPIO_writeDio(DIO_LED_RED, 0);
    GPIO_writeDio(DIO_LED_GREEN, 0);
    //GPIO_write(CONFIG_GPIO_LED_0, 0);
    //GPIO_write(CONFIG_GPIO_LED_1, 0);
    return true;
}

static uint8_t pwm_calc_sample(uint32_t cut_tick_ms, uint32_t period_ms, uint32_t duty, uint32_t phase_ms) {
    uint8_t val = 0;
    uint32_t time_saw = (phase_ms + cut_tick_ms) % period_ms;
    if((((period_ms * duty) / 100)) < time_saw) {
        val = 1;
    }

    return val;
}

bool proc_led(void) {
    uint32_t cut_tick = get_time_ms32();
    uint8_t val = pwm_calc_sample(cut_tick, greenLed.period_ms, greenLed.duty, greenLed.phase_ms);
    //GPIO_write(CONFIG_GPIO_LED_1, val);
    GPIO_writeDio(DIO_LED_GREEN, val);

#ifdef HAS_HEALTH_MONITOR
    uint8_t red_led_val = 0;
    if(HealthMon.init_error) {
        red_led_val = pwm_calc_sample(cut_tick, redLed.period_ms, redLed.duty, redLed.phase_ms);
    }
    //GPIO_write(CONFIG_GPIO_LED_0, red_led_val);
    GPIO_writeDio(DIO_LED_RED, red_led_val);
#endif /*HAS_HEALTH_MONITOR*/
    // if err turn red LED
    return true;
}
