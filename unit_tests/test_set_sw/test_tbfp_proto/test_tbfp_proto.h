#ifndef TEST_TBFP_PROTO_H
#define TEST_TBFP_PROTO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

bool test_tbfp_proto_1(void);
bool test_tbfp_types(void);

#define TEST_SUIT_TBFP_PROTO {"tbfp_proto", test_tbfp_proto_1}, {"tbfp_types", test_tbfp_types},

#ifdef __cplusplus
}
#endif

#endif /* TEST_TBFP_PROTO_H */
