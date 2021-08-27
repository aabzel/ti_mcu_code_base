#include "test_sx1262.h"

#include "sx1262_drv.h"
#include "unit_test_check.h"

 

bool test_sx1262(void) {
  uint64_t sync_word=0;
  EXPECT_TRUE( sx1262_get_sync_word(&sync_word));
  EXPECT_NE(0,sync_word);
  EXPECT_NE(0xFFFFFFFFFFFFFFFF,sync_word);

  uint32_t rand_num=0;
  EXPECT_TRUE(sx1262_get_rand(&rand_num));
  EXPECT_NE(0,rand_num);
  EXPECT_NE(0xFFFFFFFF,rand_num);

  RadioPacketTypes_t packet_type = PACKET_TYPE_UNDEF;
  EXPECT_TRUE(sx1262_get_packet_type(&packet_type) );
  EXPECT_EQ_VARS(PACKET_TYPE_GFSK, PACKET_TYPE_LORA, packet_type);
  //try undef op code command

  return true;
}

