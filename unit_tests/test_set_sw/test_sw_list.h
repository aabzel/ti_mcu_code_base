#ifndef TEST_SW_LIST_H
#define TEST_SW_LIST_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#ifdef HAS_TEST_SUIT_UTILS
#include "bit_utils_test.h"
#include "byte_utils_test.h"
#include "data_utils_test.h"
#include "float_utils_test.h"
#include "test_system.h"
#include "test_time_utils.h"
#else
#define TEST_SUIT_SYSTEM
#define TEST_SUIT_UTILS_BIT
#define TEST_SUIT_UTILS_BYTE
#define TEST_SUIT_UTILS_DATA
#define TEST_SUIT_UTILS_FLOAT
#define TEST_SUIT_UTILS_TIME
#endif

#ifdef HAS_TEST_SUIT_CONVERT
#include "test_convert.h"
#else
#define TEST_SUIT_CONVERT
#endif

#ifdef HAS_TEST_SUIT_CRC
#include "test_crc.h"
#else
#define TEST_SUIT_CRC
#endif

#ifdef HAS_TEST_SUIT_UBLOX_PROTO
#include "test_ublox_proto.h"
#else
#define UBLOX_PROTO_TEST_SUIT
#endif

#ifdef HAS_TEST_SUIT_NMEA_PROTO
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

#ifdef HAS_TEST_SUIT_FIFO
#include "test_fifo_array.h"
#include "test_fifo_char.h"
#include "test_fifo_indexer.h"
#else
#define TEST_SUIT_FIFO_ARRAY
#define TEST_SUIT_FIFO_CHAR
#define TEST_SUIT_FIFO_INDEXER
#endif

#include "unit_test_check.h"

#include "unit_test_info.h"

bool test_types(void);
bool test_array_init(void);
bool test_64bit_mult(void);
bool test_utoa_bin8(void);
bool test_float_to_uint16(void);
bool test_type_transformation(void);
bool test_array(void);
bool test_uspec_behavior(void);

#define TEST_SUIT_SW                                                                                                   \
    TEST_SUIT_UTILS_DATA                                                                                               \
    TEST_SUIT_UTILS_FLOAT                                                                                              \
    TEST_SUIT_UTILS_BIT                                                                                                \
    TEST_SUIT_UTILS_BYTE                                                                                               \
    TEST_SUIT_UTILS_TIME                                                                                               \
    TEST_SUIT_CRC                                                                                                      \
    TEST_SUIT_FIFO_INDEXER                                                                                             \
    TEST_SUIT_FIFO_CHAR                                                                                                \
    TEST_SUIT_FIFO_ARRAY                                                                                               \
    TEST_SUIT_RTCM3_PROTO                                                                                              \
    TEST_SUIT_UTILS_TIME                                                                                               \
    TEST_SUIT_SYSTEM                                                                                                   \
    NMEA_PROTO_TEST_SUIT                                                                                               \
    TEST_SUIT_CONVERT                                                                                                  \
    STRING_UTILS_TEST_SUIT                                                                                             \
    UBLOX_PROTO_TEST_SUIT{"array_init", test_array_init}, {"uspec_behavior", test_uspec_behavior},                     \
        {"array", test_array}, {"types", test_types}, {"64bit_mult", test_64bit_mult},                                 \
        {"flt_u16", test_float_to_uint16}, {"utoa_bin8", test_utoa_bin8},                                              \
        {"type_transform", test_type_transformation},

#ifdef __cplusplus
}
#endif

#endif /*TEST_SW_LIST_H*/
