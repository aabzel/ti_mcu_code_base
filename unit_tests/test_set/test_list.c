#include "test_list.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>

#include "byte_utils_test.h"
#include "bit_utils_test.h"
#include "convert.h"
#include "data_utils_test.h"
#include "debug_info.h"
#include "diag_sys.h"
#include "io_utils.h"
#include "float_utils_test.h"
#include "test_time_utils.h"
#include "test_string_utils.h"
#include "test_nmea_proto.h"
#include "test_convert.h"
#include "test_crc.h"
#include "test_sx1262.h"
#include "test_system.h"
#include "test_ublox_proto.h"
#include "unit_test_check.h"
#include "writer_generic.h"

/*Compile time assemble array */
const unit_test_info_t test_list[] = {{"type_transform", test_type_transformation},
                                DATA_UTILS_TEST_SUIT
                                FLOAT_UTILS_TEST_SUIT
                                TIME_UTILS_TEST_SUIT
                                SYSTEM_TEST_SUIT
                                NMEA_PROTO_TEST_SUIT
                                CRC_TEST_SUIT
                                TEST_SUIT_SX1262
                                CONVERT_TEST_SUIT
                                STRING_UTILS_TEST_SUIT
                                UBLOX_PROTO_TEST_SUIT
                                BYTE_UTILS_TEST_SUIT
                                BIT_UTILS_TEST_SUIT
                                {"uspec_behavior",test_uspec_behavior},
                                {"array",test_array},
                                {"64bit_mult", test_64bit_mult},
                                {"flt_u16", test_float_to_uint16},
                                {"utoa_bin8", test_utoa_bin8}};

uint32_t get_test_list_cnt(void) {
    uint32_t cnt = 0;
    cnt = sizeof(test_list) / sizeof(test_list[0]);
    return cnt;
}

static uint64_t mul64(uint32_t a, uint32_t b) {
    uint64_t out = a * b;
    return out;
}

bool test_float_to_uint16(void) {
    EXPECT_EQ(100, float_to_uint16(100.5));
    uint16_t val=0;
    for ( val = 0; val < 0xFFFA; val++) {
        EXPECT_EQ(val, float_to_uint16(0.13f + ((float)val)));
    }
    return true;
}

bool test_array(void) {
    uint8_t a=1;
    uint8_t b[4]={1,2,3,4};
    io_printf("a[b]: %u "CRLF,a[b]);
    io_printf("b[a]: %u "CRLF,b[a]);
    return true;
}

bool test_uspec_behavior(void){
    EXPECT_EQ(11, sizeof("plane_text"));
    return true;
}


bool test_utoa_bin8(void) {
    EXPECT_STREQ((char*)"1010_1010", (char*)utoa_bin8(0xaa));
    EXPECT_STREQ((char*)"0101_0101", (char*)utoa_bin8(0x55));
    return true;
}

bool test_64bit_mult(void) {
    uint64_t exp = 0x00000002CB417800;
    EXPECT_EQ(8, sizeof(uint64_t));
    uint64_t temp10x3 = mul64(1000, 12000000);
    print_mem((uint8_t*)&temp10x3, sizeof(temp10x3), false);
    //in memory          0x007841cb00000000
    //value              0x00000000cb417800
    EXPECT_EQ((uint64_t)0x00000002cb417800, temp10x3);
    // exp        3410065408 cb417800 2 3410065408 14646119404580896770
    //  temp10x3: 48         cb417800 2 3410065408 14646119404580896770!
    io_printf("\n exp: %"PRIu64"" CRLF, 0x00000002CB417800);
    print_mem((uint8_t*)&exp, 8U, false);
    print_mem((uint8_t*)&temp10x3, 8U, false);
    io_printf("\n temp10x3: %"PRIu64"" CRLF, temp10x3);
    return true;
}

static bool test_type_transformation_arg(uint16_t expected, float input) {
    EXPECT_EQ(expected, (uint16_t)(10U) * (input));
    EXPECT_EQ(expected, (uint16_t)(10.0f) * (input));
    return true;
}

bool test_type_transformation(void) {
    EXPECT_TRUE(test_type_transformation_arg(125, 12.5f));
    EXPECT_TRUE(test_type_transformation_arg(1256, 125.6f));
    return true;
}
