#ifndef TIME_DIAG_H
#define TIME_DIAG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include <time.h>

bool print_time_date(struct tm* time_date);
bool print_time(struct tm* time_date);

#ifdef __cplusplus
}
#endif

#endif /* TIME_DIAG_H */
