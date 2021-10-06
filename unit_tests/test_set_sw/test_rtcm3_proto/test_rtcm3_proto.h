#ifndef TEST_RTCM3_PROTO_H
#define TEST_RTCM3_PROTO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

bool test_rtcm3_proto(void);
bool test_rtcm3_types(void);

#define TEST_SUIT_RTCM3_PROTO {"rtcm3_proto", test_rtcm3_proto}, {"rtcm3_types", test_rtcm3_types},

#ifdef __cplusplus
}
#endif

#endif /* TEST_RTCM3_PROTO_H */