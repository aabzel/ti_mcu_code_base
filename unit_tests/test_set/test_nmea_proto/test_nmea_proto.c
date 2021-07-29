#include "test_nmea_proto.h"

#include "nmea_protocol.h"
#include "unit_test_check.h"

const char msg_gnrmc[] = "$GNRMC,072316.27,A,5551.84825,N,03725.60995,E,0.010,,290721,11.73,E,A,V*76";

// tsr nmea+
static bool test_nmea_proto_gnrmc(void) {
   rmc_t rmc= {0};
   EXPECT_TRUE( gnss_parse_rmc((char*)msg_gnrmc, &rmc));
   EXPECT_EQ(72316,rmc.utc);
   EXPECT_EQ('A',rmc.data_valid);
   EXPECT_NEAR(5551.84825,rmc.lat,0.00001);
   EXPECT_EQ('N',rmc.lat_dir);
   EXPECT_NEAR(3725.60995,rmc.lon,0.00001);
   EXPECT_EQ('E',rmc.lon_dir);
   EXPECT_NEAR(0.010,rmc.speed_knots,0.00001);
   EXPECT_EQ(290721,rmc.date);
   EXPECT_NEAR(11.73, rmc.mv, 0.00001);
   EXPECT_EQ('E',rmc.mv_ew);
   EXPECT_EQ('A',rmc.pos_mode);

   return true;
}

static bool test_nmea_checksum(void){
    uint8_t checksum = nmea_calc_checksum((char*)&msg_gnrmc[1], strlen(msg_gnrmc)-4);
    EXPECT_EQ(0x76, checksum);
    return true;
}

  
bool test_nmea_proto(void) {
  EXPECT_TRUE(test_nmea_checksum());
  EXPECT_TRUE(test_nmea_proto_gnrmc());
  EXPECT_TRUE(nmea_parse((char*) msg_gnrmc, &NmeaData));
  EXPECT_TRUE(nmea_init());
  return true;
}

