#ifndef UTILS_LOG_H
#define UTILS_LOG_H

#include <stdbool.h>
#include <stdint.h>

#include "writer_generic.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  LOG_LEVEL_UNKNOWN = -4,
  LOG_LEVEL_PARANOID = -3,
  LOG_LEVEL_DEBUG = -2,
  LOG_LEVEL_NOTICE = -1,
  LOG_LEVEL_INFO = 0,
  LOG_LEVEL_WARNING = 1,
  LOG_LEVEL_ERROR = 2,
  LOG_LEVEL_CRITICAL = 3,
  LOG_LEVEL_LAST = LOG_LEVEL_CRITICAL
} log_level_t;

typedef enum {
  UNKNOWN_FACILITY,
  ADC,
  DAC,
  DIAG,
  I2C,
  TIM,
  NVS,
  SPI,
  SYS,
  UART,
  ALL_FACILITY
} log_facility_t;

extern log_level_t log_levels[ALL_FACILITY];

extern bool log_flush;
extern bool log_colored;
extern bool log_facility_name;

#ifdef EMBEDDED_TEST
extern bool log_zero_time;
#endif

bool log_write_begin(log_level_t level, log_facility_t facility);
void log_write_end(void);
void set_log_level(log_facility_t facility, log_level_t level);
log_level_t get_log_level(log_facility_t facility);
const char *log_level_color(log_level_t level);
const char *facility2str(log_facility_t facility);

void log_write_(log_level_t level, log_facility_t facility, const char *format,
                ...);

void LOG_PARN(log_facility_t facility, const char *format, ...);
void LOG_DEBUG(log_facility_t facility, const char *format, ...);
void LOG_NOTICE(log_facility_t facility, const char *format, ...);
void LOG_INFO(log_facility_t facility, const char *format, ...);
void LOG_WARNING(log_facility_t facility, const char *format, ...);
void LOG_ERROR(log_facility_t facility, const char *format, ...);
void LOG_CRITICAL(log_facility_t facility, const char *format, ...);

#define SET_SYS_FACILITY (SYS)

#ifndef __BASE__
#define __BASE__ __FILE__
#endif

#ifdef __cplusplus
}
#endif

#endif /* UTILS_LOG_H */
