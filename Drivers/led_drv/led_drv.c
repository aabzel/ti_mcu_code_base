#include "led_drv.h"

#include <gpio.h>
#include <stddef.h>
#include <stdint.h>
#include <ti/drivers/GPIO.h>

#include "clocks.h"
#include "data_utils.h"
#include "gpio_drv.h"
#ifdef HAS_HEALTH_MONITOR
#include "health_monitor.h"
#endif /*HAS_HEALTH_MONITOR*/
#include "utils_math.h"

Led_t Led[LED_CNT];

static void test_leds(void) {
    GPIO_writeDio(DIO_LED_RED, 1);
    GPIO_writeDio(DIO_LED_GREEN, 1);
}

bool led_blink(Led_t *inLed, uint32_t duration_ms){
    bool res = false;
    if(inLed && (0<duration_ms)){
        inLed->on_time_ms = get_time_ms32();
        inLed->duration_ms= duration_ms;
        inLed->mode = LED_MODE_BLINK;
        res = gpio_set_state(inLed->dio_num, 1);
    }
    return res;
}

bool led_init(void) {
    Led[LED_INDEX_GREEN].period_ms = LED_GREEN_PERIOD_MS;
    Led[LED_INDEX_GREEN].duty = LED_DUTY;
    Led[LED_INDEX_GREEN].phase_ms = LED_PHASE;
    Led[LED_INDEX_GREEN].mode = LED_MODE_PWM;
    Led[LED_INDEX_GREEN].dio_num = DIO_LED_GREEN;

    Led[LED_INDEX_RED].period_ms = LED_RED_PERIOD_MS;
    Led[LED_INDEX_RED].duty = LED_RED_DUTY;
    Led[LED_INDEX_RED].phase_ms = LED_RED_PHASE;
    Led[LED_INDEX_RED].mode = LED_MODE_NONE;
    Led[LED_INDEX_RED].dio_num = DIO_LED_RED;

    test_leds();

    return true;
}

static bool proc_led(Led_t *inLed){
    bool res = false;
    if(inLed){
        uint32_t cur_time_ms = get_time_ms32();
        uint32_t cur_duration_ms = 0;
        if(LED_MODE_BLINK==inLed->mode){
            cur_duration_ms = cur_time_ms - inLed->on_time_ms;
            if(inLed->duration_ms < cur_duration_ms ){
                res = gpio_set_state(inLed->dio_num, 0);
                inLed->mode = LED_MODE_NONE;
            }
        }else if(LED_MODE_PWM==inLed->mode){
            uint8_t val = 0;
            val  = pwm_sample_calc_num(cur_time_ms,
                                       inLed->period_ms,
                                       inLed->duty,
                                       inLed->phase_ms);
            res = gpio_set_state(inLed->dio_num, val);
        }
    }
    return res;
}

bool proc_leds(void) {
    bool res = true;
    uint16_t i=0;
    for(i=0;i<ARRAY_SIZE(Led);i++){
        res=proc_led(&Led[i])&&res;
    }

#ifdef HAS_HEALTH_MONITOR
    if(HealthMon.init_error) {
        Led[LED_INDEX_RED].mode = LED_MODE_PWM;
    }
#endif /*HAS_HEALTH_MONITOR*/
    return res;
}
