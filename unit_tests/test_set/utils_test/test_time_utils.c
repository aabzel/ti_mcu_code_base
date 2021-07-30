#include "test_time_utils.h"

#include <stdio.h>
#include <time.h>

#include "time_utils.h"
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
  EXPECT_EQ(7,time_stamp.tm_mon);
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
  EXPECT_TRUE(test_time_utils_1());
  EXPECT_TRUE(test_time_utils_2());
  EXPECT_TRUE(test_date_utils_1());
  EXPECT_TRUE(test_date_utils_2());
  return true;
}
/*
https://www.exploringbinary.com/twos-complement-converter/
*/
