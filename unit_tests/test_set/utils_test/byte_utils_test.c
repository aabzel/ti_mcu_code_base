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


static bool test_reverse_byte_order24(void) {
  EXPECT_EQ(0x00443322, reverse_byte_order_uint24(0x00223344));
  EXPECT_EQ(0x00112233, reverse_byte_order_uint24(0x00332211));
  return true;
}


static bool test_reverse_byte_order16(void) {
  EXPECT_EQ(0x2211, reverse_byte_order_uint16(0x1122));
  return true;
}

bool test_byte_reverse(void){
  EXPECT_TRUE(test_reverse_byte_order64());
  EXPECT_TRUE(test_reverse_byte_order32());
  EXPECT_TRUE(test_reverse_byte_order24());
  EXPECT_TRUE(test_reverse_byte_order16());
  return true;
}
/*
tsr copy_rev+
*/
bool test_copy_rev(void){
    uint8_t array[8]={0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88};
    EXPECT_EQ(0x1122, copy_and_rev16( array));
    EXPECT_EQ(0x11223344, copy_and_rev32( array));
    EXPECT_EQ(0x1122334455667788, copy_and_rev64( array));
    return true;
}
