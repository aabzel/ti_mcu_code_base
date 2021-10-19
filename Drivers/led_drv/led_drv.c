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
#include "utils_math.h"

Led_t greenLed;
Led_t redLed;

static void test_leds(void) {
    GPIO_writeDio(DIO_LED_RED, 1);
    GPIO_writeDio(DIO_LED_GREEN, 1);
}

bool led_init(void) {
    greenLed.period_ms = LED_GREEN_PERIOD_MS;
    greenLed.duty = LED_DUTY;
    greenLed.phase_ms = LED_PHASE;

    redLed.period_ms = LED_RED_PERIOD_MS;
    redLed.duty = LED_RED_DUTY;
    redLed.phase_ms = LED_RED_PHASE;

    test_leds();

    return true;
}

bool proc_led(void) {
    bool res = false;
    uint8_t green_val = 0;
    uint32_t cur_time_ms = get_time_ms32();
   // float freq = 1000.0f / ((float)greenLed.period_ms);
    green_val = pwm_sample_calc_num(cur_time_ms,
                                    greenLed.period_ms,
                                    greenLed.duty,
                                    greenLed.phase_ms);
    //   t val = pwm_sample_calc(cur_time_ms, greenLed.period_ms, greenLed.duty, greenLed.phase_ms);
    res = gpio_set_state(DIO_LED_GREEN, green_val);

    uint8_t red_led_val = 0;
#ifdef HAS_HEALTH_MONITOR
    if(HealthMon.init_error) {
        //freq = 1000.0f / ((float)redLed.period_ms) ;
        red_led_val = pwm_sample_calc_num(cur_time_ms,
                                          greenLed.period_ms,
                                          redLed.duty,
                                          redLed.phase_ms);
    }
    GPIO_writeDio(DIO_LED_RED, red_led_val);
    res = gpio_set_state(DIO_LED_RED, red_led_val);
#endif /*HAS_HEALTH_MONITOR*/
    return res;
}
