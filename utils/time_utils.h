#ifndef TIME_UTILS_H
#define TIME_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include <time.h>

bool parse_time_from_val(uint32_t packed_time, struct tm* tm_stamp);
bool parse_date_from_val(uint32_t packed_date, struct tm* tm_stamp);
bool parse_date_time_from_string(char *name, struct tm* tm_stamp);

#ifdef __cplusplus
}
#endif

#endif /* TIME_UTILS_H */
