#include "log.h"

#include <inttypes.h>
#include <stdarg.h>
#include <string.h>

#ifdef X86_64
#include <stdio.h>
#endif

#include "io_utils.h"
#ifdef HAS_MCU
#ifdef HAS_CLOCK
#include "clocks.h"
#endif
#endif
#include "sys.h"
#include "terminal_codes.h"
#include "writer_generic.h"

static char log_level_name(log_level_t level);
static bool is_log_enabled(log_level_t level, log_facility_t facility);

log_level_t log_levels[ALL_FACILITY];

bool log_colored = true;
bool log_facility_name = true;
bool log_flush = false;
#ifdef HAS_MCU
#endif

char log_level_name(log_level_t level) {
    char result = 'U';
    switch(level) {
    case LOG_LEVEL_UNKNOWN:
        result = 'U';
        break;
    case LOG_LEVEL_PARANOID:
        result = 'P';
        break;
    case LOG_LEVEL_DEBUG:
        result = 'D';
        break;
    case LOG_LEVEL_NOTICE:
        result = 'N';
        break;
    case LOG_LEVEL_INFO:
        result = 'I';
        break;
    case LOG_LEVEL_WARNING:
        result = 'W';
        break;
    case LOG_LEVEL_ERROR:
        result = 'E';
        break;
    case LOG_LEVEL_CRITICAL:
        result = 'C';
        break;

    default:
        result = '?';
        break;
    }
    return result;
}

const char* log_level_color(log_level_t level) {
    const char* result = VT_SETCOLOR_NORMAL;
    switch(level) {
    case LOG_LEVEL_UNKNOWN:
        result = VT_SETCOLOR_NORMAL;
        break;
    case LOG_LEVEL_PARANOID:
        result = VT_SETCOLOR_PARANOID;
        break;
    case LOG_LEVEL_DEBUG:
        result = VT_SETCOLOR_BLUE;
        break;
    case LOG_LEVEL_NOTICE:
        result = VT_SETCOLOR_NOTICE;
        break;
    case LOG_LEVEL_INFO:
        result = VT_SETCOLOR_GREEN;
        break;
    case LOG_LEVEL_WARNING:
        result = VT_SETCOLOR_YELLOW;
        break;
    case LOG_LEVEL_ERROR:
        result = VT_SETCOLOR_RED;
        break;
    case LOG_LEVEL_CRITICAL:
        result = VT_SETCOLOR_RED;
        break;

    default:
        result = "?";
        break;
    }
    return result;
}

static bool set_log_level_all(log_level_t level) {
    bool res = true;
    uint32_t f = 0;
    for(f = 0; f < sizeof(log_levels) / sizeof(log_levels[0]); f++) {
        log_levels[f] = level;
    }
    return res;
}

bool set_log_level(log_facility_t facility, log_level_t level) {
    bool res = false;
    if(facility == ALL_FACILITY) {
        res = set_log_level_all(level);
    }
    if((UNKNOWN_FACILITY < facility) && (facility < ALL_FACILITY)) {
        log_levels[facility] = level;
        res = true;
    }
    return res;
}

log_level_t get_log_level(log_facility_t facility) {
    log_level_t level = LOG_LEVEL_UNKNOWN;
    if((UNKNOWN_FACILITY < facility) && (facility < ALL_FACILITY)) {
        level = log_levels[facility];
    }
    return level;
}

static bool is_log_enabled(log_level_t level, log_facility_t facility) {
    bool res = true;
    if(level < get_log_level(facility)) {
        res = false;
    }
    return res;
}

const char* facility2str(log_facility_t facility) {
    const char* facility_str = "x";
    switch(facility) {
    case UNKNOWN_FACILITY:
        facility_str = "?";
        break;
    case LG_FLASH:
        facility_str = "FLASH";
        break;
    case LG_INT:
        facility_str = "INT";
        break;

    case DIAG:
        facility_str = "DIAG";
        break;
    case LG_PWR:
        facility_str = "PWR";
        break;
    case BATT:
        facility_str = "BATT";
        break;
    case BOOT:
        facility_str = "BOOT";
        break;
    case FLASH_FS:
        facility_str = "FLASH_FS";
        break;
    case NMEA:
        facility_str = "NMEA";
        break;
    case I2C:
        facility_str = "I2C";
        break;
    case LED:
        facility_str = "LED";
        break;
    case LG_RTC:
        facility_str = "RTC";
        break;
    case RTCM:
        facility_str = "RTCM";
        break;
    case SYS:
        facility_str = "SYS";
        break;
    case UART:
        facility_str = "UART";
        break;
    case CRC8:
        facility_str = "CRC8";
        break;
    case CLI:
        facility_str = "CLI";
        break;
    case UBX:
        facility_str = "UBX";
        break;
    case ZED_F9P:
        facility_str = "ZED_F9P";
        break;
    case TIM:
        facility_str = "TIM";
        break;
    case TBFP:
        facility_str = "TBFP";
        break;
    case HMOM:
        facility_str = "HMON";
        break;
    case PARAM:
        facility_str = "PARAM";
        break;
    case PMIC:
        facility_str = "PMIC";
        break;
    case TEST:
        facility_str = "TEST";
        break;
    case SPI:
        facility_str = "SPI";
        break;
    case CAN:
        facility_str = "CAN";
        break;
    case LG_CAL:
        facility_str = "CALENDAR";
        break;
#ifdef X86_64
    case COM:
        facility_str = "COM";
        break;
#endif
    case LORA:
        facility_str = "LoRa";
        break;
    case WDT:
        facility_str = "WDT";
        break;
    case FIFO:
        facility_str = "FIFO";
        break;
    case ALL_FACILITY:
        facility_str = "*";
        break;
    default:
        facility_str = "??";
        break;
    }
    return facility_str;
}

bool log_write_begin(log_level_t level, log_facility_t facility) {
    bool res = false;

    if(is_log_enabled(level, facility)) {
        uint64_t now = 0;
#ifdef HAS_CLOCK
        now = get_time_ms64();
#endif
        if(log_colored) {
            io_putstr(log_level_color(level));
        }
#ifdef X86_64
        io_printf("\n");
#endif
        io_printf("%" PRIu32 ".%03" PRIu32, (uint32_t)(now / 1000), (uint32_t)(now % 1000));
        io_printf(":%c ", log_level_name(level));
        if(log_facility_name) {
            io_printf("[%s] ", facility2str(facility));
        }
        res = true;
    }
    return res;
}

void log_write_end(void) {
    if(log_colored) {
        io_putstr(VT_SETCOLOR_NORMAL);
    }
#ifdef HAS_MCU
    io_putstr(CRLF);
#endif
#ifndef NO_EMBEDED
    if(log_flush) {
#ifdef HAS_PRINTF
        flush_printf();
#endif
    }
#endif
}

#if 0
void log_write_(log_level_t level, log_facility_t facility, const char* format, ...) {
    if(log_write_begin(level, facility)) {
        va_list va;
        va_start(va, format);
        io_vprintf(format, va);
        va_end(va);
        log_write_end();
    }
}
#endif

void LOG_PARN(log_facility_t facility, const char* format, ...) {
    if(log_write_begin(LOG_LEVEL_PARANOID, facility)) {
        va_list va;
        va_start(va, format);
        io_vprintf(format, va);
        va_end(va);
        log_write_end();
    }
}

void LOG_DEBUG(log_facility_t facility, const char* format, ...) {
    if(log_write_begin(LOG_LEVEL_DEBUG, facility)) {
        va_list va;
        va_start(va, format);
        io_vprintf(format, va);
        va_end(va);
        log_write_end();
    }
}

void LOG_NOTICE(log_facility_t facility, const char* format, ...) {
    if(log_write_begin(LOG_LEVEL_NOTICE, facility)) {
        va_list va;
        va_start(va, format);
        io_vprintf(format, va);
        va_end(va);
        log_write_end();
    }
}

void LOG_INFO(log_facility_t facility, const char* format, ...) {
    if(log_write_begin(LOG_LEVEL_INFO, facility)) {
        va_list va;
        va_start(va, format);
        io_vprintf(format, va);
        va_end(va);
        log_write_end();
    }
}

void LOG_WARNING(log_facility_t facility, const char* format, ...) {
    if(log_write_begin(LOG_LEVEL_WARNING, facility)) {
        va_list va;
        va_start(va, format);
        io_vprintf(format, va);
        va_end(va);
        log_write_end();
    }
}

void LOG_ERROR(log_facility_t facility, const char* format, ...) {
    if(log_write_begin(LOG_LEVEL_ERROR, facility)) {
        va_list va;
        va_start(va, format);
        io_vprintf(format, va);
        va_end(va);
        log_write_end();
    }
}

void LOG_CRITICAL(log_facility_t facility, const char* format, ...) {
    if(log_write_begin(LOG_LEVEL_CRITICAL, facility)) {
        va_list va;
        va_start(va, format);
        io_vprintf(format, va);
        va_end(va);
        log_write_end();
    }
}
