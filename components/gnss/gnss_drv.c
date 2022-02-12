#include "gnss_drv.h"
#include "gnss_utils.h"
#include "gnss_diag.h"
#ifdef HAS_NMEA
#include "nmea_protocol.h"
#endif
#include "ublox_driver.h"
#include "log.h"
#include "time_utils.h"
#include "time_diag.h"


Gnss_t Gnss = { 0 };

bool gnss_proc(void){
    bool res = true, res_time = false, res_dot = false;
    static bool first_time = true;
    static bool first_gnss = true;

    res = is_valid_time_date(&NavInfo.date_time);
    if (res)    {
        if (first_time)        {
#ifdef HAS_LOG
                LOG_INFO(GNSS, "SpotValidTime!");
                print_time_date(&NavInfo.date_time);
      #endif
#ifdef HAS_CALENDAR
                calendar_settime(&NavInfo.date_time);
      #endif /*HAS_CALENDAR*/
            first_time = false;
        }
        Gnss.time_date = NavInfo.date_time;
        res_time = true;
    }    else    {
#ifdef HAS_LOG
            LOG_DEBUG(GNSS, "InvalUbxTimeDate");
      #endif
    }

    res = is_valid_gnss_coordinates(NavInfo.coordinate);
    if (res)    {
        if (first_gnss)        {
#ifdef HAS_LOG
                LOG_INFO(GNSS, "SpotValidGNSSData!");
                print_coordinate(NavInfo.coordinate, true);
    #endif
            first_gnss = false;
        }
        Gnss.coordinate_cur = NavInfo.coordinate;
        res_dot = true;
    }    else    {
#ifdef HAS_LOG
        LOG_DEBUG(GNSS, "InvalUbxGNSSDot");
    #endif
    }

#ifdef HAS_NMEA
    res = is_valid_time_date(&NmeaData.time_date);
    if(res) {
        if(first_time) {
  #ifdef HAS_LOG
            LOG_INFO(GNSS, "SpotValidTime!");
            print_time_date(&NmeaData.time_date);
  #endif
  #ifdef HAS_CALENDAR
            calendar_settime(&NmeaData.time_date);
  #endif /*HAS_CALENDAR*/
            first_time = false;
        }
        Gnss.time_date = NmeaData.time_date;
        res_time = true;
    } else {
  #ifdef HAS_LOG
        LOG_DEBUG(GNSS, "InvalNmeaTimeDate");
  #endif
    }


    res = is_valid_gnss_coordinates(NmeaData.coordinate_dd);
    if(res) {
        if(first_gnss) {
#ifdef HAS_LOG
            LOG_INFO(GNSS, "SpotValidGNSSData!");
            print_coordinate(NmeaData.coordinate_dd, true);
#endif
            first_gnss = false;
        }
        Gnss.coordinate_cur = NmeaData.coordinate_dd;
        res_dot = true;
    } else {
#ifdef HAS_LOG
        LOG_DEBUG(GNSS, "InvalNmeaGNSSDot");
#endif
    }
#endif /*HAS_NMEA*/

    res = is_valid_gnss_coordinates(Gnss.coordinate_cur);
    if (res)    {
        Gnss.coordinate_last = Gnss.coordinate_cur;
    }    else    {
#ifdef HAS_LOG
        LOG_ERROR(GNSS, "Inval GNSS CurCoordinate");
#endif
    }

    if(res_time && res_dot) {
        res = true;
    } else {
        res = false;
    }

    return res;
}
