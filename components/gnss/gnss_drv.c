#include "gnss_drv.h"
/*GNSS receiver invariant component*/
#include "gnss_utils.h"
#include "gnss_diag.h"
#ifdef HAS_CALENDAR
#include "calendar.h"
#endif
#ifdef HAS_NMEA
#include "nmea_protocol.h"
#endif
#ifdef HAS_UBLOX
#include "ublox_driver.h"
#endif
#include "log.h"
#include "time_utils.h"
#include "time_diag.h"


Gnss_t Gnss = { 0 };

static bool gnss_update_from_nmea(void){
    bool res = true;
    bool res_time = true;
    bool res_dot = true;
#ifdef HAS_NMEA
    res = is_valid_time_date(&NmeaData.time_date);
    if(res) {
        if(Gnss.first_time) {
  #ifdef HAS_LOG
            LOG_INFO(NMEA, "SpotValidTime!");
            print_time_date(&NmeaData.time_date);
  #endif
  #ifdef HAS_CALENDAR
            calendar_settime(&NmeaData.time_date);
  #endif /*HAS_CALENDAR*/
            Gnss.first_time = false;
        }
        Gnss.time_date = NmeaData.time_date;
        res_time = true;
    } else {
  #ifdef HAS_LOG
        LOG_DEBUG(NMEA, "InvalNmeaTimeDate");
  #endif
    }


    res = is_valid_gnss_coordinates(NmeaData.coordinate_dd);
    if(res) {
        if(Gnss.first_gnss) {
#ifdef HAS_LOG
            LOG_INFO(NMEA, "SpotValidGNSSData!");
            print_coordinate(NmeaData.coordinate_dd, true);
#endif
            Gnss.first_gnss = false;
        }
        Gnss.coordinate_cur = NmeaData.coordinate_dd;
        res_dot = true;
    } else {
#ifdef HAS_LOG
        LOG_DEBUG(NMEA, "InvalNmeaGNSSDot");
#endif
    }
#endif /*HAS_NMEA*/

    if(res_dot && res_time){
        res = true;
    }else{
        res = false;
    }
    return res;
}

#ifdef HAS_UBLOX
static bool gnss_update_from_ubx(void){
    bool res_time = true;
    bool res_dot = true;
    bool res = true;
    res = is_valid_time_date(&NavInfo.date_time);
    if (res) {
        if (Gnss.first_time) {
#ifdef HAS_LOG
                LOG_INFO(UBX, "SpotValidTime");
                print_time_date(&NavInfo.date_time);
      #endif
#ifdef HAS_CALENDAR
                calendar_settime(&NavInfo.date_time);
      #endif /*HAS_CALENDAR*/
                Gnss.first_time = false;
        }
        Gnss.time_date = NavInfo.date_time;
        res_time = true;
    }    else    {
#ifdef HAS_LOG
            LOG_DEBUG(UBX, "InvalUbxTimeDate");
      #endif
    }
    res = is_valid_gnss_coordinates(NavInfo.coordinate);
    if (res)    {
        if (Gnss.first_gnss)        {
#ifdef HAS_LOG
                LOG_INFO(UBX, "SpotValidGNSSData!");
                print_coordinate(NavInfo.coordinate, true);
#endif
                Gnss.first_gnss = false;
        }
        Gnss.coordinate_cur = NavInfo.coordinate;
        res_dot = true;
    }    else    {
#ifdef HAS_LOG
        LOG_DEBUG(UBX, "InvalUbxGNSSDot");
    #endif
    }
    if(res_dot && res_time){
        res = true;
    }else{
        res = false;
    }
    return res;
}
#endif

bool gnss_proc(void){
    bool res = true;

    res = is_valid_gnss_coordinates(Gnss.coordinate_cur);
    if (res)    {
        Gnss.coordinate_last = Gnss.coordinate_cur;
    }    else    {
#ifdef HAS_LOG
        LOG_ERROR(GNSS, "InvalGnssCurCoordinate");
#endif
    }

#ifdef HAS_NMEA
    res = gnss_update_from_nmea();
#endif
#ifdef HAS_UBLOX
    if(false==res){
       res=gnss_update_from_ubx();
    }
#endif

    return res;
}

bool gnss_init(void){
    bool res = true;
    Gnss.first_gnss=true;
    Gnss.first_time=true;
    return res;
}
