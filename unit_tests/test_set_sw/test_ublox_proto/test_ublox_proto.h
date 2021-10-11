#ifndef TEST_UBLOX_PROTO_H
#define TEST_UBLOX_PROTO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

bool test_ublox_proto_err_len(void);
bool test_ublox_proto_set_baud(void);

#define UBLOX_PROTO_TEST_SUIT {"ublox_proto_set_baud", test_ublox_proto_set_baud},\
                              {"ublox_proto_err_len", test_ublox_proto_err_len},


#ifdef __cplusplus
}
#endif

#endif /* TEST_UBLOX_PROTO_H */
