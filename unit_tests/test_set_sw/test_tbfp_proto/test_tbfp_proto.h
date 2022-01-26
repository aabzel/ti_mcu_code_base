#ifndef TEST_TBFP_PROTO_H
#define TEST_TBFP_PROTO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

bool test_tbfp_proto_flow_ctrl(void);
bool test_tbfp_proto_29(void);
bool test_tbfp_proto_auto(void);
bool test_tbfp_proto_0(void);
bool test_tbfp_proto_1(void);
bool test_tbfp_proto_2(void);
bool test_tbfp_proto_overlen(void);
bool test_tbfp_types(void);

#define TEST_SUIT_TBFP_PROTO                                                                                           \
    {"tbfpTypes", test_tbfp_types},\
    {"tbfpProtoAuto", test_tbfp_proto_auto}, \
{"tbfpProto1", test_tbfp_proto_1}, \
{"tbfp_proto2", test_tbfp_proto_2},             \
        {"tbfpProto1", test_tbfp_proto_0}, {"tbfpProto29", test_tbfp_proto_29},                                        \
        {"tbfpProto_flow_ctrl", test_tbfp_proto_flow_ctrl},

#ifdef __cplusplus
}
#endif

#endif /* TEST_TBFP_PROTO_H */
