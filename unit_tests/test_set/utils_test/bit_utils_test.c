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

static bool test_extract_subval_utils8(void) {
  EXPECT_EQ(0x07, extract_subval_from_8bit(0x0E, 3, 1));
  EXPECT_EQ(0x06, extract_subval_from_8bit(0xEF, 6, 4));
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

static bool test_bit_ctl(void) {
    uint32_t arr[64] = {0};
    EXPECT_FALSE(bit32_control_proc(&arr[0],'s',44));

    arr[0] = 0;
    EXPECT_TRUE(bit32_control_proc(&arr[0],'s',0));
    EXPECT_EQ(1,arr[0]);

    arr[0] = 0;
    EXPECT_TRUE(bit32_control_proc(&arr[0],'s',1));
    EXPECT_EQ(2,arr[0]);

    arr[0] = 0;
    EXPECT_TRUE(bit32_control_proc((uint32_t *)&arr[0],'s',2));
    EXPECT_EQ(4,arr[0]);

    arr[0] = 0;
    EXPECT_TRUE(bit32_control_proc((uint32_t *)&arr[0],'s',4));
    EXPECT_EQ(16,arr[0]);
    return true;
}

/*tsr bit_utils+*/
bool test_bit_utils(void) {
  EXPECT_TRUE(test_extract_subval_utils8());
  EXPECT_TRUE( test_bit_ctl());
  EXPECT_TRUE(test_extract_subval_utils32());
  EXPECT_TRUE(test_extract_subval_utils64());
  EXPECT_TRUE(test_insert_subval_utils32());
  return true;
}

bool test_bit_type_size(void) {
  EXPECT_EQ(1, sizeof(BitField8_t));
  EXPECT_EQ(2, sizeof(BitField16_t));
  EXPECT_EQ(4, sizeof(BitField32_t));
  EXPECT_EQ(8, sizeof(BitField64_t));
  return true;
}

bool test_bit_mask(void) {
  EXPECT_EQ(MASK_1BIT, generate_8bit_left_mask(1));
  EXPECT_EQ(MASK_3BIT, generate_8bit_left_mask(3));
  EXPECT_EQ(MASK_8BIT, generate_8bit_left_mask(8));

  EXPECT_EQ(MASK_1BIT, generate_32bit_left_mask(1));
  EXPECT_EQ(MASK_2BIT, generate_32bit_left_mask(2));
  EXPECT_EQ(MASK_4BIT, generate_32bit_left_mask(4));
  EXPECT_EQ(MASK_25BIT, generate_32bit_left_mask(25));

  EXPECT_EQ(MASK_1BIT, generate_64bit_left_mask(1));
  EXPECT_EQ(MASK_2BIT, generate_64bit_left_mask(2));
  EXPECT_EQ(MASK_4BIT, generate_64bit_left_mask(4));
  EXPECT_EQ(MASK_25BIT, generate_64bit_left_mask(25));
  EXPECT_EQ(MASK_30BIT, generate_64bit_left_mask(30));

  EXPECT_TRUE(test_generate_custom_mask());
  return true;
}

