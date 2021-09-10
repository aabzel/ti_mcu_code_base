#include "test_list.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>

#include "bit_utils_test.h"
#include "byte_utils_test.h"
#include "clocks.h"
#include "convert.h"
#include "data_utils_test.h"
#include "debug_info.h"
#include "diag_sys.h"
#include "float_utils_test.h"
#include "io_utils.h"
#include "test_convert.h"
#include "test_crc.h"
#include "test_flash_fs.h"
#include "test_nmea_proto.h"
#include "test_params.h"
#include "test_string_utils.h"
#include "test_sx1262.h"
#include "test_system.h"
#include "test_time_utils.h"
#include "test_ublox_proto.h"
#include "unit_test_check.h"
#include "writer_generic.h"

/*Compile time assemble array */
const unit_test_info_t test_list[] = {
    {"type_transform", test_type_transformation},
    TEST_SUIT_PARAMS DATA_UTILS_TEST_SUIT FLOAT_UTILS_TEST_SUIT TIME_UTILS_TEST_SUIT SYSTEM_TEST_SUIT
        NMEA_PROTO_TEST_SUIT CRC_TEST_SUIT TEST_SUIT_FLASH_FS TEST_SUIT_SX1262 CONVERT_TEST_SUIT STRING_UTILS_TEST_SUIT
            UBLOX_PROTO_TEST_SUIT BYTE_UTILS_TEST_SUIT BIT_UTILS_TEST_SUIT{"array_init", test_array_init},
    {"clock_us", test_clock_us},
    {"uspec_behavior", test_uspec_behavior},
    {"array", test_array},
    {"types", test_types},
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
    uint16_t val = 0;
    for(val = 0; val < 0xFFFA; val++) {
        EXPECT_EQ(val, float_to_uint16(0.13f + ((float)val)));
    }
    return true;
}

bool test_array(void) {
    uint8_t a = 3;
    uint8_t b[4] = {1, 2, 3, 4};
    io_printf("a[b]: %u " CRLF, a[b]);
    io_printf("b[a]: %u " CRLF, b[a]);
    EXPECT_EQ(a[b], b[a]);

    a = 0xff;
    a++;
    EXPECT_EQ(0x00, a);
    return true;
}

bool test_uspec_behavior(void) {
    EXPECT_EQ(11, sizeof("plane_text"));
    return true;
}

bool test_utoa_bin8(void) {
    EXPECT_STREQ((char*)"1010_1010", (char*)utoa_bin8(0xaa));
    EXPECT_STREQ((char*)"0101_0101", (char*)utoa_bin8(0x55));
    return true;
}
// tsr array_init
bool test_array_init(void) {
    uint8_t arr[4] = {42, 2};
    EXPECT_EQ(42, arr[0]);
    EXPECT_EQ(2, arr[1]);
    EXPECT_EQ(0, arr[2]);
    EXPECT_EQ(0, arr[3]);
    // ++i=i++; build error
    // i++=++i; build error
    return true;
}

bool test_64bit_mult(void) {
    uint64_t exp = 0x00000002CB417800;
    EXPECT_EQ(8, sizeof(uint64_t));
    uint64_t temp10x3 = mul64(1000, 12000000);
    print_mem((uint8_t*)&temp10x3, sizeof(temp10x3), false);
    // in memory          0x007841cb00000000
    // value              0x00000000cb417800
    EXPECT_EQ((uint64_t)0x00000002cb417800, temp10x3);
    // exp        3410065408 cb417800 2 3410065408 14646119404580896770
    //  temp10x3: 48         cb417800 2 3410065408 14646119404580896770!
    io_printf("\n exp: %" PRIu64 "" CRLF, 0x00000002CB417800);
    print_mem((uint8_t*)&exp, 8U, false);
    print_mem((uint8_t*)&temp10x3, 8U, false);
    io_printf("\n temp10x3: %" PRIu64 "" CRLF, temp10x3);
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

bool test_clock_us(void) {
    uint32_t i = 0;
    uint64_t up_time_us_cur = 0;
    uint64_t up_time_us_prev = 0;
    up_time_us_prev = get_time_us();
    pause_1us();
    for(i = 0; i < 10000; i++) {
        up_time_us_cur = get_time_us();
        EXPECT_GR(up_time_us_prev, up_time_us_cur, i);
        pause_1us();
        up_time_us_prev = up_time_us_cur;
    }
    return true;
}

typedef struct xSomeType_t {
    uint8_t a;
    uint8_t b;
    uint8_t c;
} SomeType_t;

bool test_types(void) {
    EXPECT_EQ(1, sizeof(char));
    EXPECT_EQ(4, sizeof(float));
    EXPECT_EQ(4, sizeof(size_t));
    EXPECT_EQ(4, sizeof(unsigned));
    EXPECT_EQ(1, sizeof(uint8_t));
    EXPECT_EQ(8, sizeof(uint64_t));
    EXPECT_EQ(4, sizeof(int));
    EXPECT_EQ(2, sizeof(short int));
    EXPECT_EQ(3, sizeof(SomeType_t));
    return true;
}
