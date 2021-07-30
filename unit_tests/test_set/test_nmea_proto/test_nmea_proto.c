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

const char msg_gnrgga[] = "$GNGGA,140212.00,5540.70555,N,03737.93437,E,1,12,0.58,201.3,M,13.3,M,,*45";

static bool test_nmea_proto_gngga(void) {
   gga_t gga= {0};
   EXPECT_TRUE( gnss_parse_gga((char*)msg_gnrgga, &gga));
   EXPECT_EQ(140212,gga.utc);
   EXPECT_NEAR(5540.70555,gga.lat,0.000001);
   EXPECT_EQ('N',gga.lat_dir);
   EXPECT_NEAR(03737.93437,gga.lon,0.00001);
   EXPECT_EQ('E',gga.lon_dir);
   EXPECT_EQ(1,gga.quality);
   EXPECT_EQ(12,gga.nb_sat);
   EXPECT_NEAR(0.58,gga.hdop,0.00001);
   EXPECT_NEAR(201.3, gga.height, 0.00001);
   EXPECT_EQ('M',gga.height_unit);
   EXPECT_NEAR(13.3, gga.geoid_separation, 0.001);
   EXPECT_EQ('M',gga.geoid_unit);

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
  EXPECT_TRUE(test_nmea_proto_gngga());
  EXPECT_TRUE(nmea_parse((char*) msg_gnrmc, &NmeaData));
  EXPECT_TRUE(nmea_parse((char*) msg_gnrgga, &NmeaData));
  EXPECT_TRUE(nmea_init());
  return true;
}

