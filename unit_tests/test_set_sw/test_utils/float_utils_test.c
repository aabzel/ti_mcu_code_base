#include "float_utils_test.h"

#include <stdio.h>

#include "float_utils.h"
#include "unit_test_check.h"

#define PRECISION 0.2f
bool test_float_utils(void) {
  float val = 0.0f;
  bool res = IS_FLOAT_EQUAL_ABSOLUTE(0.1f, 0.1f, PRECISION);
  EXPECT_TRUE(res);

  res = IS_FLOAT_EQUAL_ABSOLUTE(10.0f, 0.1f, PRECISION);
  EXPECT_FALSE(res);

  EXPECT_TRUE(is_float_equal_absolute(0.1f, 0.1f, PRECISION));

  val = float_max(2000.0f, 100.0f);
  res = IS_FLOAT_EQUAL_ABSOLUTE(2000.0f, val, PRECISION);
  EXPECT_TRUE(res);

  val = float_max(200.0f, 3000.0f);
  res = IS_FLOAT_EQUAL_ABSOLUTE(3000.0f, val, PRECISION);
  EXPECT_TRUE(res);

  return true;
}
