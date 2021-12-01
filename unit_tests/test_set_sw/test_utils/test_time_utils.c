#include "test_time_utils.h"

#include <stdio.h>
#include <time.h>

#include "float_utils.h"
#include "time_utils.h"
#include "uart_common.h"
#include "unit_test_check.h"

bool test_date_utils_1(void) {
  struct tm time_stamp = {0};
  EXPECT_FALSE(parse_date_from_val(288821,&time_stamp));
  EXPECT_FALSE(parse_date_from_val(320721,&time_stamp));
  return true;
}

bool test_date_utils_2(void) {
  struct tm time_stamp = {0};
  EXPECT_TRUE(parse_date_from_val(280721,&time_stamp));
  EXPECT_EQ(2021,time_stamp.tm_year);
  EXPECT_EQ(28,time_stamp.tm_mday);
  EXPECT_EQ(6,time_stamp.tm_mon);
  return true;
}

bool test_time_utils_1(void) {
  struct tm time_stamp = {0};
  EXPECT_FALSE(parse_time_from_val(100389,&time_stamp));
  EXPECT_FALSE(parse_time_from_val(108846,&time_stamp));
  EXPECT_FALSE(parse_time_from_val(250346,&time_stamp));
  return true;
}

bool test_time_utils_2(void) {
  struct tm time_stamp = {0};
  EXPECT_TRUE(parse_time_from_val(100346,&time_stamp));
  EXPECT_EQ(10,time_stamp.tm_hour);
  EXPECT_EQ(03,time_stamp.tm_min);
  EXPECT_EQ(46,time_stamp.tm_sec);
  return true;
}

/*tsr time_utils+*/
bool test_time_utils(void) {
  uint32_t duration_us;
  uint32_t duration_ms;
  duration_us = calc_uart_transfer_time_us(460800, 128);
  EXPECT_BETWEEN(3000,duration_us,3500);
  EXPECT_TRUE(is_float_equal_absolute(3333.3f, (float) duration_us, 100.0f));

  duration_us = calc_uart_transfer_time_us(38400, 128);
  EXPECT_BETWEEN(40000-10,duration_us,40000+10);
  EXPECT_TRUE(is_float_equal_absolute(40000.0f, (float) duration_us, 100.0f));

  duration_ms = calc_uart_transfer_time_ms(460800, 128);
  EXPECT_BETWEEN(3,duration_ms,4);
  EXPECT_TRUE(is_float_equal_absolute(3.3f, (float) duration_ms, 0.9f));

  duration_ms = calc_uart_transfer_time_ms(38400, 128);
  EXPECT_BETWEEN(40-2,duration_ms,40+2);
  EXPECT_TRUE(is_float_equal_absolute(40.0f, (float) duration_ms, 100.0f));



  EXPECT_TRUE(test_time_utils_1());
  EXPECT_TRUE(test_time_utils_2());
  EXPECT_TRUE(test_date_utils_1());
  EXPECT_TRUE(test_date_utils_2());
  return true;
}
/*
https://www.exploringbinary.com/twos-complement-converter/
*/
