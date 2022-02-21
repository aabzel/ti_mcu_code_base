#ifndef NMEA_PARSER_H
#define NMEA_PARSER_H

#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#include "gnss_utils.h"
#include "nmea_types.h"
#include "timer_utils.h"
#ifdef HAS_CLOCK
#include "clocks.h"
#endif

extern NmeaProtocol_t NmeaProto;
extern NmeaData_t NmeaData;

uint8_t nmea_calc_checksum(char* nmea_data, uint16_t len);
bool nmea_check(void);
bool nmea_init(void);
bool nmea_parse(char* nmea_msg, uint16_t len, NmeaData_t* gps_ctx);
bool gnss_parse_zda(char* nmea_msg, zda_t* zda);
bool gnss_parse_rmc(char* nmea_msg, rmc_t* rmc);
bool gnss_parse_vtg(char* nmea_msg, vtg_t* vtg);
bool gnss_parse_gga(char* nmea_msg, gga_t* gga);
bool gnss_parse_gsa(char* nmea_msg, gsa_t* gsa);
bool gnss_parse_pbux_pos(char* nmea_msg, pbux_t* const pbux);
bool gnss_parse_gll(char* nmea_msg, gll_t* gll);
bool nmea_proc(void);
bool nmea_proc_byte(uint8_t rx_byte);

#endif /* NMEA_PARSER_H */
