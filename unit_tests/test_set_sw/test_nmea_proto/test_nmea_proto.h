#ifndef TEST_NMEA_PROTO_H
#define TEST_NMEA_PROTO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

bool test_nmea_proto(void);
bool test_nmea_proto_gnzda(void);
bool test_nmea_proto_gngga(void);
bool test_nmea_proto_gnrmc(void);
bool test_nmea_proto_gngsa(void);

#define TEST_SUIT_NMEA_PROTO           \
  {"nmea_gsa", test_nmea_proto_gngsa}, \
  {"nmea_zda", test_nmea_proto_gnzda}, \
  {"nmea_rmc", test_nmea_proto_gnrmc}, \
  {"nmea_gga", test_nmea_proto_gngga}, \
  {"nmea_proto", test_nmea_proto},

#ifdef __cplusplus
}
#endif

#endif /* TEST_NMEA_PROTO_H */
