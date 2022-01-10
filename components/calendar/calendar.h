#ifndef CALENDAR_H
#define CALENDAR_H

#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#define MIN_2_SEC(MIN) ((MIN)*60)

bool calendar_init(void);
bool calendar_gettime(struct tm* date_time);
uint32_t calendar_settime(struct tm* date_time);

#endif /* CALENDAR_H  */
