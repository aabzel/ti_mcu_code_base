#include "test_sw_list.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>

#include "convert.h"
#include "io_utils.h"
#include "unit_test_check.h"
#ifdef HAS_CLI
#include "clocks.h"
#include "diag_sys.h"
#include "debug_info.h"
#include "writer_generic.h"
#endif /*HAS_CLI*/

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
#ifdef HAS_CLI
    io_printf("a[b]: %u " CRLF, a[b]);
    io_printf("b[a]: %u " CRLF, b[a]);
#endif /*HAS_CLI*/
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
#ifdef HAS_CLI
    print_mem((uint8_t*)&temp10x3, sizeof(temp10x3), false);
#endif /*HAS_CLI*/
    // in memory          0x007841cb00000000
    // value              0x00000000cb417800
    EXPECT_EQ((uint64_t)0x00000002cb417800, temp10x3);
    // exp        3410065408 cb417800 2 3410065408 14646119404580896770
    //  temp10x3: 48         cb417800 2 3410065408 14646119404580896770!
#ifdef HAS_CLI
    io_printf("\n exp: %" PRIu64 "" CRLF, 0x00000002CB417800);
    print_mem((uint8_t*)&exp, 8U, false);
    print_mem((uint8_t*)&temp10x3, 8U, false);
    io_printf("\n temp10x3: %" PRIu64 "" CRLF, temp10x3);
#endif /*HAS_CLI*/
    return true;
}

#ifdef HAS_MCU
static bool test_type_transformation_arg(uint16_t expected, float input) {
    EXPECT_EQ(expected, (uint16_t)( ((float)10U) * (input) ));
    EXPECT_EQ(expected, (uint16_t)( ((float)10.0f) * (input) ));
    return true;
}
#endif

bool test_type_transformation(void) {
#ifdef HAS_MCU
    EXPECT_TRUE(test_type_transformation_arg(125, 12.5f));
    EXPECT_TRUE(test_type_transformation_arg(1256, 125.6f));
#endif
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
