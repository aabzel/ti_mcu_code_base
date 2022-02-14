#ifndef TEST_NMEA_PROTO_H
#define TEST_NMEA_PROTO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

bool test_nmea_proto(void);
bool test_nmea_crc(void);
bool test_nmea_proto_pubx(void);
bool test_nmea_proto_gnvtg(void);
bool test_nmea_proto_gnzda(void);
bool test_nmea_proto_gngga(void);
bool test_nmea_proto_gngll(void);
bool test_nmea_proto_gnrmc(void);
bool test_nmea_proto_gngsa(void);

#define TEST_SUIT_NMEA_PROTO           \
  {"nmea_gsa", test_nmea_proto_gngsa}, \
  {"nmea_crc", test_nmea_crc}, \
  {"nmea_zda", test_nmea_proto_gnzda}, \
  {"nmea_gll", test_nmea_proto_gngll}, \
  {"nmea_pubx", test_nmea_proto_pubx}, \
  {"nmea_vtg", test_nmea_proto_gnvtg}, \
  {"nmea_rmc", test_nmea_proto_gnrmc}, \
  {"nmea_gga", test_nmea_proto_gngga}, \
  {"nmea_proto", test_nmea_proto},

#ifdef __cplusplus
}
#endif

#endif /* TEST_NMEA_PROTO_H */
