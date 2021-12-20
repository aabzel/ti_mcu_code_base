#ifndef UTILS_LOG_H
#define UTILS_LOG_H

#include <stdbool.h>
#include <stdint.h>

#include "writer_generic.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum eLog_level_t{
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

#ifdef STM32F413xx
typedef uint8_t log_facility_t;
#define UNKNOWN_FACILITY 0
#define LG_ADC 1
#define BOOT 2
#define LG_DAC 3
#define DIAG 4
#define LG_I2C 5
#define LG_FLASH 6
#define FLASH_FS 7
#define LORA 8
#define LG_NVS 9
#define TIM 10
#define SPI 11
#define PARAM 12
#define LG_RF 13
#define LG_RNG 14
#define SYS 15
#define UART 16
#define ALL_FACILITY 17
#else
typedef enum eLog_facility_t {
  UNKNOWN_FACILITY=0,
  LG_ADC,
  LG_RTC,
  BATT ,
  CAN,
  BOOT,
  LG_DAC,
  DIAG,
  I2C,
  INT,
  NMEA,
  HMOM,
  TEST,
  TBFP,
  LG_FLASH,
  FLASH_FS,
  LORA,
  NVS,
  TIM,
  RF,
  RTC,
  SPI,
  PARAM,
  LG_PWR,
  ZED_F9P,
  LG_RNG,
  SYS,
  UBX,
  UART,
  ALL_FACILITY
} log_facility_t;
#endif /*STM32F413xx*/

extern log_level_t log_levels[ALL_FACILITY];

extern bool log_flush;
extern bool log_colored;
extern bool log_facility_name;

#ifdef EMBEDDED_TEST
extern bool log_zero_time;
#endif

bool log_write_begin(log_level_t level, log_facility_t facility);
void log_write_end(void);
bool set_log_level(log_facility_t facility, log_level_t level);
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
