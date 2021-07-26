#include "data_utils_test.h"

#include <stdio.h>

#include "data_utils.h"
#include "unit_test_check.h"
/*tsr data_utils+*/
bool test_data_utils(void) {
  EXPECT_EQ(6, two_complement_to_decimal(0x06, 7));
  EXPECT_EQ(-15, two_complement_to_decimal(0x71, 7));
  EXPECT_EQ(-23, two_complement_to_decimal(0x69, 7));
  return true;
}
/*
https://www.exploringbinary.com/twos-complement-converter/
*/