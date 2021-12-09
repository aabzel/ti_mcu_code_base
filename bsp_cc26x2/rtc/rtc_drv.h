#ifndef RTC_DRV_H
#define RTC_DRV_H

#include <stdbool.h>
#include <stdint.h>


#define XTAL_RTC 32768

bool rtc_init(void);
uint32_t rtc_get_ms(void);
bool rtc_set_sec(uint32_t sec);

#endif /* RTC_DRV_H  */
