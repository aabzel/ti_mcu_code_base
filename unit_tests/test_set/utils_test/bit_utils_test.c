#include "bit_utils_test.h"

#include "bit_utils.h"
#include "unit_test_check.h"

static bool test_generate_custom_mask(void) {
  EXPECT_EQ(0x0000000F, generate_32bit_custom_mask(3, 0));
  EXPECT_EQ(0x000000F0, generate_32bit_custom_mask(7, 4));
  EXPECT_EQ(0x00FFFF00, generate_32bit_custom_mask(8 + 16 - 1, 8));
  return true;
}

static bool test_insert_subval_utils32(void) {
  uint32_t val = 0x00000000;
  val = insert_subval_in_32bit(val, 0xD0, 31, 21);
  EXPECT_EQ(0x00D0, extract_subval_from_32bit(val, 31, 21));

  val = insert_subval_in_32bit(val, 0xC6, 15, 5);
  EXPECT_EQ(0x00C6, extract_subval_from_32bit(val, 15, 5));
  EXPECT_EQ(0x00D0, extract_subval_from_32bit(val, 31, 21));

  return true;
}

static bool test_extract_subval_utils32(void) {
  EXPECT_EQ(0x0034, extract_subval_from_32bit(0x5344, 11, 4));
  EXPECT_EQ(0x000f, extract_subval_from_32bit(0xF000, 15, 12));
  EXPECT_EQ(0x000f, extract_subval_from_32bit(0x0F00, 11, 8));
  EXPECT_EQ(0x000f, extract_subval_from_32bit(0x000F, 3, 0));
  return true;
}

static bool test_extract_subval_utils64(void) {
  EXPECT_EQ(0x0034, extract_subval_from_64bit(0x5344, 11, 4));
  EXPECT_EQ(0x000f, extract_subval_from_64bit(0xF000, 15, 12));
  EXPECT_EQ(0x000f, extract_subval_from_64bit(0x0F00, 11, 8));
  EXPECT_EQ(0x000f, extract_subval_from_64bit(0x000F, 3, 0));
  return true;
}
/*tsr bit_utils+*/
bool test_bit_utils(void) {
  EXPECT_EQ(0x0001, generate_32bit_left_mask(1));
  EXPECT_EQ(0x0003, generate_32bit_left_mask(2));
  EXPECT_EQ(0x000f, generate_32bit_left_mask(4));
  EXPECT_EQ(MASK_25BIT, generate_32bit_left_mask(25));
  EXPECT_TRUE(test_extract_subval_utils32());

  EXPECT_EQ(0x0001, generate_64bit_left_mask(1));
  EXPECT_EQ(0x0003, generate_64bit_left_mask(2));
  EXPECT_EQ(0x000f, generate_64bit_left_mask(4));
  EXPECT_EQ(MASK_25BIT, generate_64bit_left_mask(25));
  EXPECT_EQ(MASK_30BIT, generate_64bit_left_mask(30));
  EXPECT_TRUE(test_extract_subval_utils64());


  EXPECT_TRUE(test_generate_custom_mask());
  EXPECT_TRUE(test_insert_subval_utils32());
  return true;
}
