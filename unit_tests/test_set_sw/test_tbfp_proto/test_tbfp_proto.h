#ifndef TEST_TBFP_PROTO_H
#define TEST_TBFP_PROTO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

bool test_tbfp_proto_29(void);
bool test_tbfp_proto_0(void);
bool test_tbfp_proto_overlen(void);
bool test_tbfp_proto_1(void);
bool test_tbfp_proto_2(void);
bool test_tbfp_types(void);

#define TEST_SUIT_TBFP_PROTO         \
 {"tbfp_types", test_tbfp_types},    \
 {"tbfp_proto1", test_tbfp_proto_0}, \
 {"tbfp_proto29", test_tbfp_proto_29}, \
 {"tbfp_proto1", test_tbfp_proto_1}, \
 {"tbfp_proto2", test_tbfp_proto_2},

#ifdef __cplusplus
}
#endif

#endif /* TEST_TBFP_PROTO_H */
