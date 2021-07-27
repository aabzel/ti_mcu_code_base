#include "test_ublox_proto.h"

#include "ubx_protocol.h"
#include "unit_test_check.h"

const uint8_t ubx_baud_115200_message[28] = {
  0xB5, 0x62,
  0x06, 0x00,
  0x14, 0x00,
  0x01, 0x00, 0x00, 0x00, 0xD0,
  0x08, 0x00, 0x00, 0x00, 0xC2,
  0x01, 0x00, 0x03, 0x00, 0x03,
  0x00, 0x00, 0x00, 0x00, 0x00,
  0xBC, 0x5E};
/*tsr ublox_proto+*/
static bool test_ublox_proto_set_baud  (void) {
   uint8_t i=0;

   ubx_reset_rx();
   UbloxPorotocol.rx_pkt_cnt=0;
   for(i=0; i<sizeof(ubx_baud_115200_message); i++) {
       ubx_proc_byte(ubx_baud_115200_message[i]);
   }

   EXPECT_EQ(1, UbloxPorotocol.rx_pkt_cnt);

   return true;
}
  
bool test_ublox_proto(void) {
  EXPECT_TRUE(test_ublox_proto_set_baud());
  return true;
}

