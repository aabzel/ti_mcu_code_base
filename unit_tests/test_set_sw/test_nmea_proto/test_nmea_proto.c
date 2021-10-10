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
   EXPECT_NEAR(5551.84825l,rmc.lat,1e-6);
   EXPECT_EQ('N',rmc.lat_dir);
   EXPECT_NEAR(3725.60995l,rmc.lon,0.00001f);
   EXPECT_EQ('E',rmc.lon_dir);
   EXPECT_NEAR(0.010l,rmc.speed_knots,0.00001f);
   EXPECT_EQ(290721,rmc.date);
   EXPECT_NEAR(11.73l, rmc.mv, 0.00001f);
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

const char msg_gnrgll[] = "$GNGLL,5540.70584,N,03737.93404,E,140125.00,A,A*74";
static bool test_nmea_proto_gngll(void) {
    gll_t gll= {0};
    EXPECT_TRUE( gnss_parse_gll((char*)msg_gnrgll, &gll));
    EXPECT_EQ(140125,gll.time);
    EXPECT_NEAR(5540.70584,gll.lat,0.000001);
    EXPECT_EQ('N',gll.lat_dir);
    EXPECT_NEAR(03737.93404,gll.lon,0.00001);
    EXPECT_EQ('E',gll.lon_dir);
    EXPECT_EQ('A',gll.status);
    EXPECT_EQ('A',gll.pos_mode);

    return true;
}

const char msg_gnrgsa[] = "$GNGSA,A,3,78,85,68,84,69,,,,,,,,1.04,0.58,0.86,2*0B";
static bool test_nmea_proto_gngsa(void) {
    gsa_t gsa= {0};
    EXPECT_TRUE( gnss_parse_gsa((char*)msg_gnrgsa, &gsa));
    EXPECT_EQ('A',gsa.opMode);
    EXPECT_EQ(3,gsa.navMode);
    EXPECT_NEAR(1.04,gsa.PDOP,0.000001);
    EXPECT_NEAR(0.58,gsa.HDOP,0.00001);
    EXPECT_NEAR(0.86,gsa.VDOP,0.00001);
    EXPECT_EQ(2,gsa.systemId);

    return true;
}

const char msg_gnrvtg[] = "$GNVTG,,T,,M,0.017,N,0.032,K,A*3A";
static bool test_nmea_proto_gnvtg(void) {
    vtg_t vtg= {0};
    EXPECT_TRUE( gnss_parse_vtg((char*)msg_gnrvtg, &vtg));
    EXPECT_EQ('T',vtg.cogtUnit);
    EXPECT_EQ('M',vtg.cogmUnit);
    EXPECT_NEAR(0.017,vtg.sogn,0.000001);
    EXPECT_EQ('N',vtg.sognUnit);
    EXPECT_NEAR(0.032,vtg.sogk,0.00001);
    EXPECT_EQ('K',vtg.sogkUnit);
    EXPECT_EQ('A',vtg.posMode);
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
  EXPECT_TRUE(test_nmea_proto_gnvtg());
  EXPECT_TRUE(test_nmea_proto_gngga());
  EXPECT_TRUE(test_nmea_proto_gngll());
  EXPECT_TRUE(test_nmea_proto_gngsa());
  EXPECT_TRUE(nmea_parse((char*) msg_gnrmc, &NmeaData));
  EXPECT_TRUE(nmea_parse((char*) msg_gnrgga, &NmeaData));
  EXPECT_TRUE(nmea_parse((char*) msg_gnrgll, &NmeaData));
  EXPECT_TRUE(nmea_parse((char*) msg_gnrgsa, &NmeaData));
  EXPECT_TRUE(nmea_init());
  return true;
}

