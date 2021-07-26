#include "byte_utils_test.h"

#include <stdio.h>

#include "byte_utils.h"
#include "unit_test_check.h"

static bool test_reverse_byte_order64(void) {
  EXPECT_EQ(0x8877665544332211, reverse_byte_order_uint64(0x1122334455667788));
  return true;
}
static bool test_reverse_byte_order32(void) {
  EXPECT_EQ(0x44332211, reverse_byte_order_uint32(0x11223344));
  return true;
}
static bool test_reverse_byte_order16(void) {
  EXPECT_EQ(0x2211, reverse_byte_order_uint16(0x1122));
  return true;
}

bool test_byte_utils(void) {
  EXPECT_TRUE(test_reverse_byte_order64());
  EXPECT_TRUE(test_reverse_byte_order32());
  EXPECT_TRUE(test_reverse_byte_order16());
  return true;
}
