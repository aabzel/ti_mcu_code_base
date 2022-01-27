#include "led_drv.h"

#include <stddef.h>
#include <stdint.h>

#ifdef HAS_FREE_RTOS
#include <FreeRTOS.h>
#include <task.h>
#endif
#ifdef HAS_LOG
#include "io_utils.h"
#include "log.h"
#endif
#include "clocks.h"
#include "data_utils.h"
#include "gpio_drv.h"
#ifdef HAS_HEALTH_MONITOR
#include "health_monitor.h"
#endif /*HAS_HEALTH_MONITOR*/
#include "sys_config.h"
#include "utils_math.h"

Led_t Led[LED_COUNT];

static void test_leds(void) {
#if(DIO_LED_RED != DIO_LED_HEARTBEAT)
    gpio_set_state(DIO_LED_RED, 1);
#endif
#ifdef DIO_LED_HEARTBEAT
    gpio_set_state(DIO_LED_HEARTBEAT, 1);
#endif
}

#if(0 == LED_COUNT)
#error "Lack of LEDS in configs"
#endif

bool led_blink(Led_t* inLed, uint32_t duration_ms) {
    bool res = false;
    if(inLed && (0 < duration_ms)) {
        inLed->on_time_ms = get_time_ms32();
        inLed->duration_ms = duration_ms;
        inLed->mode = LED_MODE_BLINK;
        res = gpio_set_state(inLed->dio_num, 1);
    }
    return res;
}

static bool proc_led(Led_t* inLed) {
    bool res = false;
    if(inLed) {
        uint32_t cur_time_ms = get_time_ms32();
        uint8_t val = 0;
        switch(inLed->mode) {
        case LED_MODE_ON: {
            val = 1;
            res = true;
        } break;
        case LED_MODE_OFF: {
            val = 0;
            res = true;
        } break;
        case LED_MODE_BLINK: {
            uint32_t cur_duration_ms = 0;
            cur_duration_ms = cur_time_ms - inLed->on_time_ms;
            if(inLed->duration_ms < cur_duration_ms) {
                res = true;
                val = 0;
                inLed->mode = LED_MODE_NONE;
            }
        } break;
        case LED_MODE_PWM: {
            val = pwm_sample_calc_num(cur_time_ms, inLed->period_ms, inLed->duty, inLed->phase_ms);
            res = true;
        } break;
        default: {
            res = false;
        } break;
        }

        if(res) {
            res = gpio_set_state(inLed->dio_num, val);
        }
    }
    return res;
}

bool proc_leds(void) {
#ifdef HAS_LOG
    LOG_DEBUG(LED, "%s()", __FUNCTION__);
#endif
    bool res = true;
    uint16_t i = 0;
    for(i = 0; i < ARRAY_SIZE(Led); i++) {
        res = proc_led(&Led[i]) && res;
    }

#ifdef HAS_HEALTH_MONITOR
    if(HealthMon.init_error) {
#ifdef DIO_LED_RED
        Led[LED_INDEX_RED].mode = LED_MODE_PWM;
#endif
    }
#endif /*HAS_HEALTH_MONITOR*/
    return res;
}

bool led_on(Led_t* inLed) {
    bool res = false;
    if(inLed) {
        res = true;
        inLed->mode = LED_MODE_ON;
    }
    return res;
}

bool led_off(Led_t* inLed) {
    bool res = false;
    if(inLed) {
        res = true;
        inLed->mode = LED_MODE_OFF;
    }
    return res;
}

#ifdef HAS_FREE_RTOS

static void led_thread(void* arg) {
    for(;;) {
        proc_leds();
        vTaskDelay(50 / portTICK_RATE_MS);
    }
}

void led_create_task(void) { xTaskCreate(led_thread, "LED", 5000, NULL, 0, NULL); }
#endif /*HAS_TIRTOS*/

bool led_init(void) {
#ifdef HAS_LOG
    LOG_DEBUG(LED, "%s()", __FUNCTION__);
#endif
#ifdef DIO_LED_HEARTBEAT
    Led[LED_INDEX_HEARTBEAT].period_ms = LED_GREEN_PERIOD_MS;
    Led[LED_INDEX_HEARTBEAT].duty = LED_DUTY;
    Led[LED_INDEX_HEARTBEAT].phase_ms = LED_PHASE;
    Led[LED_INDEX_HEARTBEAT].mode = LED_MODE_PWM;
    Led[LED_INDEX_HEARTBEAT].dio_num = DIO_LED_HEARTBEAT;
#endif

#if(DIO_LED_RED != DIO_LED_HEARTBEAT)
    Led[LED_INDEX_RED].period_ms = LED_RED_PERIOD_MS;
    Led[LED_INDEX_RED].duty = LED_RED_DUTY;
    Led[LED_INDEX_RED].phase_ms = LED_RED_PHASE;
    Led[LED_INDEX_RED].mode = LED_MODE_NONE;
    Led[LED_INDEX_RED].dio_num = DIO_LED_RED;
#endif

    test_leds();
#ifdef HAS_FREE_RTOS
    led_create_task();
#endif

    return true;
}
