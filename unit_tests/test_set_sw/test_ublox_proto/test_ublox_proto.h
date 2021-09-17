#ifndef TEST_UBLOX_PROTO_H
#define TEST_UBLOX_PROTO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

bool test_ublox_proto(void);

#define UBLOX_PROTO_TEST_SUIT {"ublox_proto", test_ublox_proto},

#ifdef __cplusplus
}
#endif

#endif /* TEST_UBLOX_PROTO_H */
