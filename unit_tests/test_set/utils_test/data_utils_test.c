#include "data_utils_test.h"

#include <stdio.h>

#include "data_utils.h"
#include "unit_test_check.h"

bool test_extract_digit(void) {
  EXPECT_EQ( 0,extract_digit(10256,6));
  EXPECT_EQ( 9,extract_digit(100389,0));
  EXPECT_EQ( 8,extract_digit(100389,1));
  EXPECT_EQ( 3,extract_digit(100389,2));
  EXPECT_EQ( 0,extract_digit(100389,3));
  EXPECT_EQ( 5,extract_digit(10256,1));
  return true;
}

/*tsr data_utils+*/
bool test_data_utils(void) {

  EXPECT_EQ( 1,ipow(2,0));
  EXPECT_EQ( 2,ipow(2,1));
  EXPECT_EQ( 2,ipow(32,5));
  EXPECT_EQ( 4096,ipow(2,12));

  EXPECT_EQ(6, two_complement_to_decimal(0x06, 7));
  EXPECT_EQ(-15, two_complement_to_decimal(0x71, 7));
  EXPECT_EQ(-23, two_complement_to_decimal(0x69, 7));
  EXPECT_TRUE( test_extract_digit( ));
  return true;
}
/*
https://www.exploringbinary.com/twos-complement-converter/
*/
