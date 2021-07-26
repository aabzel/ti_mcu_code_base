#include "test_string_utils.h"

#include "str_utils.h"
#include "unit_test_check.h"

static bool test_replaic_char(void) {
  char temp[]="us_1_0x55aa";
  int changes = replace_char(temp, '_', ' ');
  EXPECT_EQ(2, changes);
  EXPECT_STREQ("us 1 0x55aa", temp);
  return true;
}

bool test_string_utils(void) {
  EXPECT_TRUE(test_replaic_char());


  return true;
}
