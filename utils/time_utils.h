#ifndef TIME_UTILS_H
#define TIME_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include <time.h>

int32_t time_date_cmp(struct tm* date_time1, struct tm* date_time2);
uint32_t time_calc_diff(struct tm* date_time1, struct tm* date_time2);
bool is_time_date_equal(struct tm* date_time1, struct tm* date_time2);
bool is_valid_time_date(struct tm* date_time);
bool is_valid_time(struct tm* date_time);
struct tm* time_get_time(void);
bool parse_time_from_val(uint32_t packed_time, struct tm* tm_stamp);
bool parse_date_from_val(uint32_t packed_date, struct tm* tm_stamp);
bool parse_date_time_from_string(char* name, struct tm* tm_stamp);
bool time_get_time_str(char* str, uint32_t size);
bool time_data_parse(struct tm* date_time, char* str);
bool time_parse(struct tm* date_time, char* str);
bool date_parse(struct tm* date_time, char* str);

#ifdef __cplusplus
}
#endif

#endif /* TIME_UTILS_H */
