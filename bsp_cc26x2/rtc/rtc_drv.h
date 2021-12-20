#ifndef RTC_DRV_H
#define RTC_DRV_H

#include <stdbool.h>
#include <stdint.h>
#include <time.h>


#define XTAL_RTC 32768

typedef struct xSwRtc_t{
    struct tm date_time;
    volatile uint32_t raw_sec; /*incremented in ISR*/
}SwRtc_t;

extern SwRtc_t SwRtc;

bool rtc_init(void);
bool rtc_set_sec(uint32_t sec);
uint32_t rtc_get_ms(void);

#endif /* RTC_DRV_H  */
