#include "test_nmea_proto.h"

#include "gnss_utils.h"
#include "nmea_protocol.h"
#include "unit_test_check.h"

const char msg_gnrmc[] = "$GNRMC,072316.27,A,5551.84825,N,03725.60995,E,0.010,,290721,11.73,E,A,V*76";
// tsr nmea+
bool test_nmea_proto_gnrmc(void) {
   rmc_t rmc= {0};
   EXPECT_TRUE( gnss_parse_rmc((char*)msg_gnrmc, &rmc));
   EXPECT_EQ(7,rmc.time_date.tm_hour);
   EXPECT_EQ(23,rmc.time_date.tm_min);
   EXPECT_EQ(16,rmc.time_date.tm_sec);
   EXPECT_EQ('A',rmc.data_valid);
   EXPECT_NEAR(5551.84825l,rmc.coordinate_ddmm.latitude,1e-6);
   EXPECT_EQ('N',rmc.lat_dir);
   EXPECT_NEAR(3725.60995l,rmc.coordinate_ddmm.longitude,0.00001f);
   EXPECT_EQ('E',rmc.lon_dir);
   EXPECT_NEAR(0.010l,rmc.speed_knots,0.00001f);

   EXPECT_EQ(29,rmc.time_date.tm_mday);
   EXPECT_EQ(7,rmc.time_date.tm_mon);
   EXPECT_EQ(2021,rmc.time_date.tm_year);

   EXPECT_NEAR(11.73l, rmc.mv, 0.00001f);
   EXPECT_EQ('E',rmc.mv_ew);
   EXPECT_EQ('A',rmc.pos_mode);

   return true;
}

const char msg_gnrgga[] = "$GNGGA,140212.00,5540.70555,N,03737.93437,E,1,12,0.58,201.3,M,13.3,M,,*45";
bool test_nmea_proto_gngga(void) {
   gga_t gga= {0};
   EXPECT_TRUE( gnss_parse_gga((char*)msg_gnrgga, &gga));
   EXPECT_EQ(14,gga.time_date.tm_hour);
   EXPECT_EQ(2,gga.time_date.tm_min);
   EXPECT_EQ(12,gga.time_date.tm_sec);
   EXPECT_NEAR(5540.70555,gga.coordinate_ddmm.latitude,0.000001);
   EXPECT_EQ('N',gga.lat_dir);
   EXPECT_NEAR(03737.93437,gga.coordinate_ddmm.longitude,0.00001);
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
    EXPECT_EQ(14,gll.time_date.tm_hour);
    EXPECT_EQ(1,gll.time_date.tm_min);
    EXPECT_EQ(25,gll.time_date.tm_sec);
    EXPECT_NEAR(5540.70584,gll.coordinate_ddmm.latitude,0.000001);
    EXPECT_EQ('N',gll.lat_dir);
    EXPECT_NEAR(03737.93404,gll.coordinate_ddmm.longitude,0.00001);
    EXPECT_EQ('E',gll.lon_dir);
    EXPECT_EQ('A',gll.status);
    EXPECT_EQ('A',gll.pos_mode);

    return true;
}

const char msg_gnrgsa[] = "$GNGSA,A,3,78,85,68,84,69,,,,,,,,1.04,0.58,0.86,2*0B";
bool test_nmea_proto_gngsa(void) {
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

const char msg_gnzda[] ="$GNZDA,122013.00,29,11,2021,00,00*71";
bool test_nmea_proto_gnzda(void) {
#ifdef X86_64
   printf("\n%s()",__FUNCTION__);
#endif
    zda_t zda= {0};
    EXPECT_TRUE( gnss_parse_zda((char*)msg_gnzda, &zda));
    EXPECT_EQ(12, zda.time_date.tm_hour);
    EXPECT_EQ(20, zda.time_date.tm_min);
    EXPECT_EQ(13, zda.time_date.tm_sec);
    EXPECT_EQ(29, zda.time_date.tm_mday);
    EXPECT_EQ(11, zda.time_date.tm_mon);
    EXPECT_EQ(2021, zda.time_date.tm_year);

    EXPECT_EQ(0, zda.ltzh);
    EXPECT_EQ(0, zda.ltzn);

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

const char msg_pubx[] = "$PUBX,00,001417.00,0000.00000,N,00000.00000,E,0.000,NF,5303356,3750039,0.000,0.00,0.000,,99.99,99.99,99.99,0,0,0*21";
static bool test_nmea_proto_pubx(void) {
    pbux_t pbux= {0};
    EXPECT_TRUE( gnss_parse_pbux_pos((char*)msg_pubx, &pbux));
    EXPECT_EQ(0,pbux.msg_id);//00,001417.00,0000.00000,N,00000.00000,E,0.000,NF,5303356,3750039,0.000,0.00,0.000,,99.99,99.99,99.99,0,0,0*21
    EXPECT_EQ(17,pbux.time_date.tm_sec);//001417.00,0000.00000,N,00000.00000,E,0.000,NF,5303356,3750039,0.000,0.00,0.000,,99.99,99.99,99.99,0,0,0*21
    EXPECT_EQ(14,pbux.time_date.tm_min);
    EXPECT_EQ(0,pbux.time_date.tm_hour);
#if 0
    EXPECT_NEAR(0000.0,pbux.latitude,0.000001);//0000.00000,N,00000.00000,E,0.000,NF,5303356,3750039,0.000,0.00,0.000,,99.99,99.99,99.99,0,0,0*21
    EXPECT_NEAR(0000.0,pbux.longitude,0.000001);//00000.00000,E,0.000,NF,5303356,3750039,0.000,0.00,0.000,,99.99,99.99,99.99,0,0,0*21
    EXPECT_EQ('N',pbux.ns_indicator);//N,00000.00000,E,0.000,NF,5303356,3750039,0.000,0.00,0.000,,99.99,99.99,99.99,0,0,0*21
    EXPECT_EQ('E',pbux.ee_indicator);//E,0.000,NF,5303356,3750039,0.000,0.00,0.000,,99.99,99.99,99.99,0,0,0*21
    EXPECT_NEAR(0.0,vtg.altitude,0.00001);//
    EXPECT_STR_EQ("NF",pbux.nav_status);//
    EXPECT_NEAR(5303356, pbux.horizontal_accuracy_estimate,0.3);//
    EXPECT_NEAR(3750039, pbux.vertical_accuracy_estimate,0.3);//
    EXPECT_NEAR(0.0,pbux.speed_over_ground,0.4);//km/h
    EXPECT_NEAR(0.0, pbux.Course_over_ground,0.3);//deg
    EXPECT_NEAR(0.0, pbux.Vertical_velocity,0.3);//m/s
    EXPECT_EQ('A',pbux.posMode);
#endif
    return true;
}

bool test_nmea_proto(void) {
#ifdef X86_64
   printf("\n%s()",__FUNCTION__);
#endif
  EXPECT_EQ(16, sizeof(GnssCoordinate_t));

  EXPECT_TRUE(test_nmea_proto_pubx());
  EXPECT_TRUE(test_nmea_checksum());
  EXPECT_TRUE(test_nmea_proto_gnvtg());
  EXPECT_TRUE(test_nmea_proto_gngll());
  EXPECT_TRUE(nmea_parse((char*) msg_gnrmc, &NmeaData));
  EXPECT_TRUE(nmea_parse((char*) msg_gnrgga, &NmeaData));
  EXPECT_TRUE(nmea_parse((char*) msg_gnrgll, &NmeaData));
  EXPECT_TRUE(nmea_parse((char*) msg_gnrgsa, &NmeaData));
  EXPECT_TRUE(nmea_init());
  return true;
}

