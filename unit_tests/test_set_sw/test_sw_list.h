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

#ifdef HAS_CONVERT_TEST_SUIT
#include "test_convert.h"
#else
#define CONVERT_TEST_SUIT
#endif

#ifdef HAS_CRC_TEST_SUIT
#include "test_crc.h"
#else
#define CRC_TEST_SUIT
#endif

#ifdef HAS_UBLOX_PROTO_TEST_SUIT
#include "test_ublox_proto.h"
#else
#define UBLOX_PROTO_TEST_SUIT
#endif

#ifdef HAS_NMEA_PROTO_TEST_SUIT
#include "test_nmea_proto.h"
#else
#define NMEA_PROTO_TEST_SUIT
#endif

#ifdef HAS_STRING_UTILS_TEST_SUIT
#include "test_string_utils.h"
#else
#define STRING_UTILS_TEST_SUIT
#endif


#ifdef HAS_TEST_SUIT_RTCM3_PROTO
#include "test_rtcm3_proto.h"
#else
#define TEST_SUIT_RTCM3_PROTO
#endif

#include "test_fifo_char.h"
#include "test_fifo_indexer.h"
#include "test_system.h"
#include "test_time_utils.h"
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
    TEST_SUIT_FIFO_INDEXER\
    TEST_SUIT_FIFO_CHAR \
    TEST_SUIT_RTCM3_PROTO\
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
