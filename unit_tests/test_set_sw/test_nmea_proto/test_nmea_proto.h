#ifndef TEST_NMEA_PROTO_H
#define TEST_NMEA_PROTO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

bool test_nmea_proto(void);

#define TEST_SUIT_NMEA_PROTO {"nmea_proto", test_nmea_proto},

#ifdef __cplusplus
}
#endif

#endif /* TEST_NMEA_PROTO_H */
