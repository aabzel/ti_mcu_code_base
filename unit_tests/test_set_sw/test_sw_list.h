#ifndef TEST_SW_LIST_H
#define TEST_SW_LIST_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "bit_utils_test.h"
#include "byte_utils_test.h"
#include "data_utils_test.h"
#include "float_utils_test.h"
#include "test_convert.h"
#include "test_crc.h"
#include "test_nmea_proto.h"
#include "test_string_utils.h"
#include "test_system.h"
#include "test_time_utils.h"
#include "test_ublox_proto.h"
#include "unit_test_check.h"
#include "writer_generic.h"

#include "unit_test_info.h"

bool test_types(void);
bool test_array_init(void);
bool test_64bit_mult(void);
bool test_utoa_bin8(void);
bool test_float_to_uint16(void);
bool test_type_transformation(void);
bool test_array(void) ;
bool test_uspec_behavior(void);

#define TEST_SUIT_SW \
    {"type_transform", test_type_transformation}, \
    DATA_UTILS_TEST_SUIT \
    FLOAT_UTILS_TEST_SUIT \
    TIME_UTILS_TEST_SUIT \
    SYSTEM_TEST_SUIT \
    NMEA_PROTO_TEST_SUIT \
    CRC_TEST_SUIT \
    TIME_UTILS_TEST_SUIT \
    CONVERT_TEST_SUIT\
    STRING_UTILS_TEST_SUIT\
    UBLOX_PROTO_TEST_SUIT\
    BYTE_UTILS_TEST_SUIT\
    BIT_UTILS_TEST_SUIT\
    {"array_init", test_array_init},\
    {"uspec_behavior", test_uspec_behavior},\
    {"array", test_array},\
    {"types", test_types},\
    {"64bit_mult", test_64bit_mult},\
    {"flt_u16", test_float_to_uint16},\
    {"utoa_bin8", test_utoa_bin8},\

#ifdef __cplusplus
}
#endif

#endif /*TEST_SW_LIST_H*/
