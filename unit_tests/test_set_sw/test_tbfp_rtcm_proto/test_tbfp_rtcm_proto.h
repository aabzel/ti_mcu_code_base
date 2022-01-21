#ifndef TEST_TBFP_TBFP_PROTO_H
#define TEST_TBFP_TBFP_PROTO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

bool test_tbfp_rtcm_path(void);

#define TEST_SUIT_TBFP_RTCM_STACK            \
    {"tbfp_rtcm_path", test_tbfp_rtcm_path},

#ifdef __cplusplus
}
#endif

#endif /* TEST_TBFP_TBFP_PROTO_H */
