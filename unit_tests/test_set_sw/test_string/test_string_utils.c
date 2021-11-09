#include "test_string_utils.h"

#ifdef X86_64
#include <stdio.h>
#endif /*X86_64*/

#include "str_utils.h"
#include "data_utils.h"
#include "str_utils_ex.h"
#include "unit_test_check.h"

static bool test_replaic_char(void) {
#ifdef X86_64
    printf("\n%s():", __FUNCTION__);
#endif
  char temp[]="us_1_0x55aa";
  int changes = replace_char(temp, '_', ' ');
  EXPECT_EQ(2, changes);
  EXPECT_STREQ("us 1 0x55aa", temp);
  return true;
}

bool test_string_utils(void) {
#ifdef X86_64
    printf("\n%s():", __FUNCTION__);
#endif
  EXPECT_TRUE(test_replaic_char());

  return true;
}

static const char out_array2_exp[2]={0xb1,0xe4};
static const char out_array1_exp[16]={0x94,0x09,0x94,0x20,0x1d,0x1e,0x00,0x12,
                              0x13,0x17,0x00,0x5f,0x0d,0x9f,0xf2,0x04};
static const char testStrArray[]=" 0x940994201d1e00121317005f0d9ff204 0xb1e4";


bool test_count_substring(void){
    EXPECT_EQ(2,count_substring(testStrArray,"0x"));
    return true;
}

static bool test_parse_hex_array_after_prefix(void){
#ifdef X86_64
    printf("\n%s():", __FUNCTION__);
#endif
    char out_array[16] = { 0 };
    EXPECT_EQ(2,count_substring(testStrArray,"0x"));
    EXPECT_TRUE(parse_lhex_array_after_prefix("0x",0, testStrArray, out_array, 16));
    EXPECT_EQ_MEM(out_array, out_array1_exp, 16);

    Type16Union_t un16={0};
    EXPECT_TRUE( parse_lhex_array_after_prefix("0x",1,testStrArray,&un16.u8[0],2));
    EXPECT_EQ_MEM(&un16.u8[0], out_array2_exp, 2);
    EXPECT_EQ( 0xb1e4,reverse_byte_order_uint16(un16.u16))
    return true;
}

bool test_string_utils_ext(void) {
#ifdef X86_64
    printf("\n%s():", __FUNCTION__);
#endif
  EXPECT_TRUE(test_parse_hex_array_after_prefix());

  return true;
}
