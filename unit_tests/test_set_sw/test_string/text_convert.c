#include "test_convert.h"

#include <math.h>
#include <stdint.h>

#include "convert.h"
#include "oprintf.h"

#include "unit_test_check.h"

bool test_convert_try_str2int8(void) {
    int8_t value;
    EXPECT_FALSE(try_str2int8("", &value));
    EXPECT_FALSE(try_str2int8("-129", &value));
    EXPECT_FALSE(try_str2int8("128", &value));

    EXPECT_TRUE(try_str2int8("-128", &value));
    EXPECT_EQ(INT8_MIN, value);
    EXPECT_TRUE(try_str2int8("0", &value));
    EXPECT_EQ(0, value);
    EXPECT_TRUE(try_str2int8("127", &value));
    EXPECT_EQ(INT8_MAX, value);
    return true;
}

bool test_convert_try_str2uint8_invalid(void) {
    uint8_t value;
    EXPECT_FALSE(try_str2uint8("", &value));
    EXPECT_FALSE(try_str2uint8("A", &value));
    EXPECT_FALSE(try_str2uint8("zzz", &value));
    return true;
}

bool test_convert_is_dec_str(void) {
    EXPECT_TRUE(is_dec_str("1234", 4));
    EXPECT_FALSE(is_dec_str("0x1234", 6));
    EXPECT_FALSE(is_dec_str("Fabc", 4));
    EXPECT_FALSE(is_dec_str("qwer", 6));
    return true;
}

bool test_convert_is_hex_str(void) {
    uint8_t shift_value = 0U;
    EXPECT_TRUE(is_hex_str("0xFabc", 6, &shift_value));
    EXPECT_EQ(2U, shift_value);

    EXPECT_FALSE(is_hex_str("1234", 4, &shift_value));

    EXPECT_TRUE(is_hex_str("0x1234", 6, &shift_value));
    EXPECT_EQ(2U, shift_value);

    EXPECT_TRUE(is_hex_str("Fabc", 4, &shift_value));
    EXPECT_EQ(0U, shift_value);
    EXPECT_FALSE(is_hex_str("0xqwer", 6, &shift_value));
    return true;
}

bool test_convert_try_str2uint8(void) {
    uint8_t value;
    EXPECT_FALSE(try_str2uint8("256", &value));
    EXPECT_FALSE(try_str2uint8("-1", &value));

    EXPECT_TRUE(try_str2uint8("0", &value));
    EXPECT_EQ(0, value);
    EXPECT_TRUE(try_str2uint8("255", &value));
    EXPECT_EQ(UINT8_MAX, value);
    EXPECT_TRUE(try_str2uint8("0xFF", &value));
    EXPECT_EQ(UINT8_MAX, value);
    return true;
}

bool test_convert_try_strl2uint16_hex(void) {
    uint16_t value;
    EXPECT_FALSE(try_strl2uint16_hex("", 0, &value));
    EXPECT_FALSE(try_strl2uint16_hex("-1", 2, &value));
    EXPECT_FALSE(try_strl2uint16_hex("0", 0, &value));

    EXPECT_TRUE(try_strl2uint16_hex("1234", 4, &value));
    EXPECT_EQ(0x1234, value);

    EXPECT_TRUE(try_strl2uint16_hex("FFF", 3, &value));
    EXPECT_EQ(0xFFF, value);

    EXPECT_TRUE(try_strl2uint16_hex("0", 1, &value));
    EXPECT_EQ(0, value);

    EXPECT_TRUE(try_strl2uint16_hex("FF", 2, &value));
    EXPECT_EQ(0xFF, value);
    return true;
}

bool test_convert_try_strl2uint8_hex(void) {
    uint8_t value;
    EXPECT_FALSE(try_strl2uint8_hex("", -1, &value));
    EXPECT_FALSE(try_strl2uint8_hex("FFF", -1, &value));
    EXPECT_FALSE(try_strl2uint8_hex("-1", -1, &value));
    EXPECT_FALSE(try_strl2uint8_hex("0", 0, &value));

    EXPECT_TRUE(try_strl2uint8_hex("0", -1, &value));
    EXPECT_EQ(0, value);
    EXPECT_TRUE(try_strl2uint8_hex("FF", -1, &value));
    EXPECT_EQ(UINT8_MAX, value);
    return true;
}

bool test_convert_try_str2int16(void) {
    int16_t value;
    EXPECT_FALSE(try_str2int16("", &value));
    EXPECT_FALSE(try_str2int16("-327679", &value));
    EXPECT_FALSE(try_str2int16("32768", &value));

    EXPECT_TRUE(try_str2int16("-32768", &value));
    EXPECT_EQ(INT16_MIN, value);
    EXPECT_TRUE(try_str2int16("0", &value));
    EXPECT_EQ(0, value);
    EXPECT_TRUE(try_str2int16("32767", &value));
    EXPECT_EQ(INT16_MAX, value);
    return true;
}

bool test_convert_try_str2uint16(void) {
    uint16_t value;
    EXPECT_FALSE(try_str2uint16("", &value));
    EXPECT_FALSE(try_str2uint16("65536", &value));
    EXPECT_FALSE(try_str2uint16("-1", &value));

    value = 0U;
    EXPECT_TRUE(try_str2uint16("0xABCD", &value));
    EXPECT_EQ((uint16_t)43981, value);

    EXPECT_TRUE(try_str2uint16("0", &value));
    EXPECT_EQ(0, value);
    EXPECT_TRUE(try_str2uint16("65535", &value));
    EXPECT_EQ(UINT16_MAX, value);
    return true;
}

bool test_convert_try_str2int32(void) {
    int32_t value;
    EXPECT_FALSE(try_str2int32("", &value));
    EXPECT_FALSE(try_str2int32("-2147483649", &value));
    EXPECT_FALSE(try_str2int32("2147483648", &value));

    EXPECT_TRUE(try_str2int32("-2147483648", &value));
    EXPECT_EQ(INT32_MIN, value);
    EXPECT_TRUE(try_str2int32("0", &value));
    EXPECT_EQ(0, value);
    EXPECT_TRUE(try_str2int32("2147483647", &value));
    EXPECT_EQ(INT32_MAX, value);

    EXPECT_TRUE(try_str2int32("0x12345678", &value));
    EXPECT_EQ(0x12345678, value);
    EXPECT_TRUE(try_str2int32("0x78ABCDEF", &value));
    EXPECT_EQ(0x78ABCDEF, value);
    EXPECT_TRUE(try_str2int32("0x78abcdef", &value));
    EXPECT_EQ(0x78ABCDEF, value);
    EXPECT_TRUE(try_str2int32("0x7FFFFFFF", &value));
    EXPECT_EQ(0x7FFFFFFF, value);
    EXPECT_FALSE(try_str2int32("0x8FFFFFFF", &value));
    EXPECT_FALSE(try_str2int32("0x7QFFFFFF", &value));
    EXPECT_FALSE(try_str2int32("0x7/FFFFFF", &value));
    return true;
}

bool test_convert_try_str2uint32(void) {
    uint32_t value;
    EXPECT_FALSE(try_str2uint32("", &value));
    EXPECT_FALSE(try_str2uint32("4294967296", &value));
    EXPECT_FALSE(try_str2uint32("-1", &value));

    EXPECT_TRUE(try_str2uint32("0", &value));
    EXPECT_EQ(0u, value);
    EXPECT_TRUE(try_str2uint32("4294967295", &value));
    EXPECT_EQ(UINT32_MAX, value);
    return true;
}

bool test_convert_try_str2luint32_hex(void) {
    uint32_t value;
    EXPECT_FALSE(try_strl2uint32_hex("0x12345", 7, &value));
    EXPECT_FALSE(try_strl2uint32_hex("12345", 0, &value));
    EXPECT_TRUE(try_strl2uint32_hex("12345", -1, &value));
    EXPECT_EQ(0x12345U, value);
    EXPECT_FALSE(try_strl2uint32_hex("12345~", -1, &value));
    EXPECT_FALSE(try_strl2uint32_hex("123456789", -1, &value));
    return true;
}

bool test_convert_try_str2luint32(void) {
    uint32_t value;
    EXPECT_FALSE(try_strl2uint32("12345", 0, &value));
    EXPECT_TRUE(try_strl2uint32("12345", -1, &value));
    EXPECT_EQ(12345U, value);
    EXPECT_FALSE(try_strl2uint32("12345~", -1, &value));
    EXPECT_TRUE(try_strl2uint32("12345A", -1, &value));
    EXPECT_EQ(0x12345A, value);
    return true;
}

bool test_convert_try_str2lint32(void) {
    int32_t value = -1;
    char inStr[40]="";

    strncpy(inStr, "words and 987", sizeof(inStr));
    EXPECT_FALSE(try_strl2int32(inStr, strlen(inStr), &value));
    EXPECT_EQ(0, value);

    strncpy(inStr, "hs", sizeof(inStr));
    EXPECT_FALSE(try_strl2int32(inStr, strlen(inStr), &value));
    EXPECT_EQ(0, value);

    strncpy(inStr, "20000000000000000000", sizeof(inStr));
    EXPECT_FALSE(try_strl2int32(inStr, strlen(inStr), &value));
    EXPECT_EQ(0, value);

    strncpy(inStr, "-91283472332", sizeof(inStr));
    EXPECT_FALSE(try_strl2int32(inStr, strlen(inStr), &value));
    EXPECT_EQ(0, value);

    strncpy(inStr, "+1", sizeof(inStr));
    EXPECT_TRUE(try_strl2int32(inStr, strlen(inStr), &value));
    EXPECT_EQ(1, value);

    strncpy(inStr, "4193 with words", sizeof(inStr));
    EXPECT_FALSE(try_strl2int32(inStr, strlen(inStr), &value));
    EXPECT_EQ(0, value);

    strncpy(inStr, "-42", sizeof(inStr));
    EXPECT_TRUE(try_strl2int32(inStr, strlen(inStr), &value));
    EXPECT_EQ(-42, value);

    EXPECT_FALSE(try_strl2int32("12345", 0, &value));
    EXPECT_TRUE(try_strl2int32("12345", -1, &value));
    EXPECT_EQ(12345, value);
    EXPECT_FALSE(try_strl2int32("12345~", -1, &value));
    EXPECT_TRUE(try_strl2int32("12345A", -1, &value));
    EXPECT_EQ(0x12345A, value);
    EXPECT_FALSE(try_strl2int32("12345678901234567", -1, &value));
    return true;
}

bool test_convert_try_str2uint32_hex(void) {
    uint32_t value;
    EXPECT_TRUE(try_str2uint32_hex("abCd", &value));
    EXPECT_EQ((uint32_t)43981, value);

    EXPECT_TRUE(try_str2uint32_hex("12345", &value));
    EXPECT_EQ(0x12345U, value);
    return true;
}

bool test_convert_try_str2lint64(void) {
    int64_t value;
    EXPECT_FALSE(try_strl2int64("12345", 0, &value));
    EXPECT_TRUE(try_strl2int64("12345", -1, &value));
    EXPECT_EQ(12345, value);
    EXPECT_FALSE(try_strl2int64("12345~", -1, &value));
    return true;
}

bool test_convert_try_str2luint64(void) {
    uint64_t value;
    EXPECT_FALSE(try_strl2uint64("12345", 0, &value));
    EXPECT_TRUE(try_strl2uint64("12345", -1, &value));
    EXPECT_EQ(12345U, value);
    EXPECT_FALSE(try_strl2uint64("12345~", -1, &value));
    return true;
}

bool test_convert_try_str2int64(void) {
    int64_t value;
    EXPECT_FALSE(try_str2int64("", &value));
    EXPECT_FALSE(try_str2int64("-", &value));
    EXPECT_FALSE(try_str2int64("-9223372036854775808", &value));
    EXPECT_FALSE(try_str2int64("9223372036854775808", &value));
    EXPECT_FALSE(try_str2int64("0xabcdefgh", &value));
    EXPECT_FALSE(try_str2int64("0x10000000000000000", &value));

    EXPECT_TRUE(try_str2int64("-9223372036854775807", &value));
    EXPECT_EQ(-INT64_MAX, value);

    EXPECT_TRUE(try_str2int64("9223372036854775807", &value));
    EXPECT_EQ(INT64_MAX, value);

    EXPECT_TRUE(try_str2int64("0X123456789ABCDEF", &value));
    EXPECT_EQ(0x123456789ABCDEF, value);

    EXPECT_TRUE(try_str2int64("0x123456789abcdef", &value));
    EXPECT_EQ(0x123456789abcdef, value);

    EXPECT_TRUE(try_str2int64("0x7FFFFFFFFFFFFFF", &value));
    EXPECT_EQ(0x7ffffffffffffff, value);
    return true;
}

bool test_convert_try_str2uint64(void) {
    uint64_t value;
    EXPECT_FALSE(try_str2uint64("", &value));
    EXPECT_FALSE(try_str2uint64("-", &value));
    EXPECT_FALSE(try_str2uint64("18446744073709551616", &value));
    EXPECT_FALSE(try_str2uint64("0xabcdefgh", &value));
    EXPECT_FALSE(try_str2uint64("0x100000000000000000", &value));

    EXPECT_TRUE(try_str2uint64("0", &value));
    EXPECT_EQ(0U, value);

    EXPECT_TRUE(try_str2uint64("18446744073709551615", &value));
    EXPECT_EQ(UINT64_MAX, value);

    EXPECT_TRUE(try_str2uint64("0X123456789ABCDEF", &value));
    EXPECT_EQ(0x123456789ABCDEFULL, value);

    EXPECT_TRUE(try_str2uint64("0x123456789abcdef", &value));
    EXPECT_EQ(0x123456789abcdefULL, value);

    EXPECT_TRUE(try_str2uint64("0x7FFFFFFFFFFFFFF", &value));
    EXPECT_EQ(0x7ffffffffffffffULL, value);

    EXPECT_TRUE(try_str2uint64("0xFFFFFFFFFFFFFFF", &value));
    EXPECT_EQ(0xfffffffffffffffULL, value);
    return true;
}

bool test_convert_try_str2float(void) {
    float value = 0;
    float* valueNull = NULL;

    EXPECT_TRUE(try_str2float("1", &value));
    EXPECT_NEAR(1.0f, value, 0.01f);

    EXPECT_TRUE(try_str2float("2e10", &value));
    EXPECT_NEAR(2e10, value, 0.01);

    EXPECT_TRUE(try_str2float(".1", &value));
    EXPECT_NEAR(0.1f, value, 0.01);

    EXPECT_TRUE(try_str2float("0.1", &value));
    EXPECT_NEAR(0.1f, value, 0.01);

    EXPECT_TRUE(try_str2float("3.", &value));
    EXPECT_NEAR(3.0f, value, 0.01);

    EXPECT_TRUE(try_str2float("0", &value));
    EXPECT_NEAR(0.0f, value, 0.01);

    EXPECT_TRUE(try_str2float(".2e35", &value));
    EXPECT_NEAR(2e34, value, 1e27);

    EXPECT_FALSE(try_str2float(".1.", &value));
    EXPECT_FALSE(try_str2float("53K", &value));
    EXPECT_FALSE(try_str2float(" 4e3.", &value));
    EXPECT_FALSE(try_str2float("0..", &value));
    EXPECT_FALSE(try_str2float(".  ", &value));
    EXPECT_FALSE(try_str2float("abc", &value));
    EXPECT_FALSE(try_str2float("0e", &value));
    EXPECT_FALSE(try_str2float("1 4", &value));
    EXPECT_FALSE(try_str2float("1t", &value));
    EXPECT_FALSE(try_str2float("078332e437", &value));
    EXPECT_FALSE(try_str2float("1e1000", &value));
    EXPECT_FALSE(try_str2float("1e50", &value));
    EXPECT_FALSE(try_str2float("-1e50", &value));
    EXPECT_FALSE(try_str2float("1e-50", &value));

    EXPECT_TRUE(try_str2float("12345678", &value));
    EXPECT_NEAR(12345678, value, 0.0001);

    EXPECT_TRUE(try_str2float("1e-1", &value));
    EXPECT_NEAR(0.1F, value, 0.0001);

    EXPECT_TRUE(try_str2float("1e2", &value));
    EXPECT_NEAR(100, value, 0.0001);

    EXPECT_TRUE(try_str2float("0.0", &value));
    EXPECT_NEAR(0.0, value, 0.0001);

    EXPECT_TRUE(try_str2float("-17", &value));
    EXPECT_NEAR(-17.0, value, 0.0001);

    EXPECT_FALSE(try_str2float("-28", valueNull));
    return true;
}

bool test_convert_try_strl2double(void) {
    double value = 0.0;
    EXPECT_TRUE(try_strl2double("5551.85827,N,03725.60947,E", 10, &value));
    EXPECT_NEAR(5551.85827, value, 0.0001);
    return true;
}

bool test_convert_try_str2double(void) {
    double value;
    EXPECT_FALSE(try_str2double("", &value));
    EXPECT_FALSE(try_str2double("1t", &value));
    EXPECT_FALSE(try_str2double("1e1000", &value));
    EXPECT_FALSE(try_str2double("1e-1000", &value));
    EXPECT_TRUE(try_str2double("1e50", &value));
    EXPECT_TRUE(try_str2double("1e-50", &value));

    EXPECT_FALSE(try_str2double("1e10000000", &value));
    EXPECT_FALSE(try_str2double("1e-10000000", &value));

    EXPECT_TRUE(try_str2double("12345678", &value));
    EXPECT_NEAR(12345678, value, 0.0001);
    EXPECT_TRUE(try_str2double("1e-1", &value));
    EXPECT_NEAR(0.1, value, 0.0001);
    EXPECT_TRUE(try_str2double("1e+1", &value));
    EXPECT_NEAR(10, value, 0.0001);
    EXPECT_TRUE(try_str2double("1e2", &value));
    EXPECT_NEAR(100, value, 0.0001);

    EXPECT_TRUE(try_str2double("+1e2", &value));
    EXPECT_NEAR(100, value, 0.0001);

    EXPECT_TRUE(try_str2double("-1e2", &value));
    EXPECT_NEAR(-100, value, 0.0001);

    EXPECT_TRUE(try_str2double("+1.2e2", &value));
    EXPECT_NEAR(120, value, 0.0001);

    EXPECT_TRUE(try_str2double("21055406075", &value));
    EXPECT_NEAR(21055406075, value, 0.0001);
    return true;
}

bool test_convert_try_str2bool(void) {
    bool value;
    EXPECT_TRUE(try_str2bool("yes", &value));
    EXPECT_TRUE(value);
    EXPECT_TRUE(try_str2bool("true", &value));
    EXPECT_TRUE(value);
    EXPECT_TRUE(try_str2bool("on", &value));
    EXPECT_TRUE(value);
    EXPECT_TRUE(try_str2bool("1", &value));
    EXPECT_TRUE(value);
    EXPECT_TRUE(try_str2bool("no", &value));
    EXPECT_FALSE(value);
    EXPECT_TRUE(try_str2bool("false", &value));
    EXPECT_FALSE(value);
    EXPECT_TRUE(try_str2bool("off", &value));
    EXPECT_FALSE(value);
    EXPECT_TRUE(try_str2bool("0", &value));
    EXPECT_FALSE(value);
    EXPECT_FALSE(try_str2bool(NULL, &value));
    EXPECT_FALSE(try_str2bool("xxx", &value));
    return true;
}

bool test_convert_Ltoa32(void) {
    EXPECT_STREQ("0", rx_ltoa64(0));
    EXPECT_STREQ("-1", rx_ltoa64(-1));
    EXPECT_STREQ("9223372036854775807", rx_ltoa64(INT64_MAX));
    EXPECT_STREQ("-9223372036854775808", rx_ltoa64(INT64_MIN));
    return true;
}

bool test_convert_Ltoa64(void) {
    EXPECT_STREQ("0", rx_ltoa64(0));
    EXPECT_STREQ("-1", rx_ltoa64(-1));
    EXPECT_STREQ("9223372036854775807", rx_ltoa64(INT64_MAX));
    EXPECT_STREQ("-9223372036854775808", rx_ltoa64(INT64_MIN));
    return true;
}

bool test_convert_ULtoa32(void) {
    EXPECT_STREQ("0", rx_utoa64(0));
    EXPECT_STREQ("18446744073709551615", rx_utoa64(UINT64_MAX));
    return true;
}

bool test_convert_ULtoa64(void) {
    EXPECT_STREQ("0", rx_utoa64(0));
    EXPECT_STREQ("18446744073709551615", rx_utoa64(UINT64_MAX));
    return true;
}

bool test_convert_Ltoa32_(void) {
    uint32_t len;
    char str[132];
    EXPECT_STREQ("0", ltoa32_(0, str, 10, &len));
    EXPECT_EQ(len, strlen(str));
    EXPECT_STREQ("-1", ltoa32_(-1, str, 10, &len));
    EXPECT_EQ(len, strlen(str));
    EXPECT_STREQ("2147483647", ltoa32_(INT32_MAX, str, 10, &len));
    EXPECT_EQ(len, strlen(str));
    EXPECT_STREQ("-2147483648", ltoa32_(INT32_MIN, str, 10, &len));
    EXPECT_EQ(len, strlen(str));
    return true;
}

bool test_convert_Ltoa64_(void) {
    uint32_t len;
    char str[132];
    EXPECT_STREQ("0", ltoa64_(0, str, 10, &len));
    EXPECT_EQ(len, strlen(str));
    EXPECT_STREQ("-1", ltoa64_(-1, str, 10, &len));
    EXPECT_EQ(len, strlen(str));
    EXPECT_STREQ("9223372036854775807", ltoa64_(INT64_MAX, str, 10, &len));
    EXPECT_EQ(len, strlen(str));
    EXPECT_STREQ("-9223372036854775808", ltoa64_(INT64_MIN, str, 10, &len));
    EXPECT_EQ(len, strlen(str));
    return true;
}

bool test_convert_ULtoa32_(void) {
    uint32_t len;
    char str[132];
    EXPECT_STREQ("0", utoa32_(0, str, 10, &len));
    EXPECT_EQ(1U, len);
    EXPECT_EQ(len, strlen(str));
    EXPECT_STREQ("4294967295", utoa32_(UINT32_MAX, str, 10, &len));
    EXPECT_EQ(len, strlen(str));
    return true;
}

bool test_convert_ULtoa64_(void) {
    uint32_t len;
    char str[132];
    EXPECT_STREQ("0", utoa64_(0, str, 10, &len));
    EXPECT_EQ(1U, len);
    EXPECT_EQ(len, strlen(str));
    EXPECT_STREQ("18446744073709551615", utoa64_(UINT64_MAX, str, 10, &len));
    EXPECT_EQ(len, strlen(str));
    return true;
}

bool test_convert_ULtoa_hex64(void) {
    EXPECT_STREQ("0", utoa_hex64(0));
    EXPECT_STREQ("FFFFFFFFFFFFFFFF", utoa_hex64(UINT64_MAX));
    EXPECT_STREQ("123456789ABCDEF0", utoa_hex64(0x123456789ABCDEF0UL));
    return true;
}

bool test_convert_ULtoa_hex32(void) {
    EXPECT_STREQ("0", utoa_hex32(0));
    EXPECT_STREQ("FFFFFFFF", utoa_hex32(UINT32_MAX));
    EXPECT_STREQ("ABCDEF01", utoa_hex32(0xABCDEF01));
    return true;
}

bool test_convert_str_sizeof(void) {
    EXPECT_EQ(16U, sizeof("255.255.255.255"));
    return true;
}

bool test_convert_dtoa(void) {
    char str[100]="";
    double val = 12345678912345678.;
    dtoa_(val, -1, str);
    EXPECT_STREQ("12345678912345678", str);

    val = 1234567891234567.;
    dtoa_(val, -1, str);
    EXPECT_STREQ("1234567891234567", str);

    val = 123456789123456.;
    dtoa_(val, -1, str);
    EXPECT_STREQ("123456789123456", str);

    val = 12345678912345.;
    dtoa_(val, -1, str);
    EXPECT_STREQ("12345678912345", str);

    val = 1234567891234.;
    dtoa_(val, -1, str);
    EXPECT_STREQ("1234567891234", str);

    val = 123456789123.;
    dtoa_(val, -1, str);
    EXPECT_STREQ("123456789123", str);

    val = 12345678912.;
    dtoa_(val, -1, str);
    EXPECT_STREQ("12345678912", str);

    val = 1234567891.2;
    dtoa_(val, -1, str);
    EXPECT_STREQ("1234567891", str);

    val = 123456789.12;
    dtoa_(val, -1, str);
    EXPECT_STREQ("123456789", str);

    val = 12345678.912;
    dtoa_(val, -1, str);
    EXPECT_STREQ("12345679", str);

    val = 1234567.8912;
    dtoa_(val, -1, str);
    EXPECT_STREQ("1234567.9", str);

    val = 123456.78912;
    dtoa_(val, -1, str);
    EXPECT_STREQ("123456.79", str);

    val = 12345.678912;
    dtoa_(val, -1, str);
    EXPECT_STREQ("12345.679", str);

    val = 1234.5678912;
    dtoa_(val, -1, str);
    EXPECT_STREQ("1234.5679", str);

    val = 123.45678912;
    dtoa_(val, -1, str);
    EXPECT_STREQ("123.45679", str);

    val = 12.345678912;
    dtoa_(val, -1, str);
    EXPECT_STREQ("12.345679", str);

    val = 1.2345678912;
    dtoa_(val, -1, str);
    EXPECT_STREQ("1.2345679", str);

    val = 0.12345678912;
    dtoa_(val, -1, str);
    EXPECT_STREQ("0.12345679", str);

    val = 0.012345678912;
    dtoa_(val, -1, str);
    EXPECT_STREQ("0.012345679", str);

    val = 0.0012345678912;
    dtoa_(val, -1, str);
    EXPECT_STREQ("0.0012345679", str);

    val = 1e20;
    dtoa_(val, -1, str);
#if defined(EMBEDDED_TEST)
    EXPECT_STREQ("18446744073709551615", str);
#else
    EXPECT_STREQ("0", str);
#endif
    return true;
}

bool test_convert_dtoa_trim_zero(void) {
    char str[100]="";
    double val = 100;
    dtoa_(val, -1, str);
    EXPECT_STREQ("100", str);
    return true;
}

bool test_convert_dtoa_nan(void) {
    char str[100]="";
    double val = NAN;
    dtoa_(val, -1, str);
    EXPECT_STREQ("NAN", str);
    return true;
}

/*fails*/
bool test_convert_dtoa_inf(void) {
    char str[100]="";
    double val = INFINITY;
    dtoa_(val, -1, str);
    EXPECT_STREQ("INF", str);
    return true;
}

bool test_convert_ftoa_nan(void) {
    char str[100]="";
    float val = NAN;
    ftoa_(val, -1, str);
    EXPECT_STREQ("NAN", str);
    return true;
}

bool test_convert_ftoa_inf(void) {
    char str[100]="";
    float val = INFINITY;
    ftoa_(val, -1, str);
    EXPECT_STREQ("INF", str);
    return true;
}

bool test_convert_rx_dtoa(void) {
    double t = 1e10;
    EXPECT_STREQ("10000000000", rx_dtoa(t));
    return true;
}

bool test_convert_ltoa_hex64(void) {
    int64_t t = 0x1234567890ABCDEF;
    const char* str = ltoa_hex64(t);
    EXPECT_STREQ("1234567890ABCDEF", str);

    str = ltoa_hex64(-1);
    EXPECT_STREQ("-1", str);
    return true;
}

bool test_convert_bool2name(void) {
    EXPECT_STREQ("on", bool2name(true));
    EXPECT_STREQ("off", bool2name(false));
    EXPECT_STREQ("on", bool2name(55U));
    EXPECT_STREQ("on", bool2name(1U));
    EXPECT_STREQ("off", bool2name(0U));
    return true;
}

bool test_convert_utoa_bin32(void) {
    EXPECT_STREQ("1010_1010_1010_1010_1010_1010_1010_1010", utoa_bin32(0xAAAAAAAA));
    EXPECT_STREQ("0101_0101_0101_0101_0101_0101_0101_0101", utoa_bin32(0x55555555));
    EXPECT_STREQ("0000_0000_0000_0000_0000_0000_0000_0000", utoa_bin32(0x00000000));
    EXPECT_STREQ("1111_1111_1111_1111_1111_1111_1111_1111", utoa_bin32(0xFFFFFFFF));
    EXPECT_STREQ("0001_0010_0011_0100_0101_0110_0111_1000", utoa_bin32(0x12345678));
    return true;
}

bool test_convert_utoa_bin24(void) {
    EXPECT_STREQ("1010_1010_1010_1010_1010_1010", utoa_bin24(0xAAAAAA));
    EXPECT_STREQ("0101_0101_0101_0101_0101_0101", utoa_bin24(0x555555));
    EXPECT_STREQ("0000_0000_0000_0000_0000_0000", utoa_bin24(0x000000));
    EXPECT_STREQ("1111_1111_1111_1111_1111_1111", utoa_bin24(0xFFFFFF));
    EXPECT_STREQ("0011_0100_0101_0110_0111_1000", utoa_bin24(0x345678));
    return true;
}

bool test_convert_utoa_bin16(void) {
    EXPECT_STREQ("1010_1010_1010_1010", utoa_bin16(0xAAAA));
    EXPECT_STREQ("0101_0101_0101_0101", utoa_bin16(0x5555));
    EXPECT_STREQ("0000_0000_0000_0000", utoa_bin16(0x0000));
    EXPECT_STREQ("1111_1111_1111_1111", utoa_bin16(0xFFFF));
    EXPECT_STREQ("0001_0010_0011_0100", utoa_bin16(0x1234));
    return true;
}

bool test_convert_assemble_uint32(void) {
    uint32_t qword_val = 0x00000000;
    qword_val = assemble_uint32(0x01, 0x02, 0x03, 0x04);
    EXPECT_EQ(0x01020304, qword_val);
    qword_val = assemble_uint32(0x04, 0x03, 0x02, 0x01);
    EXPECT_EQ(0x04030201, qword_val);
    return true;
}
