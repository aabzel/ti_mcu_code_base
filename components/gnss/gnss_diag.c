#include "gnss_diag.h"
// http://traveleu.ru/map/GPSconverter.htm
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "gnss_utils.h"
#include "io_utils.h"
#include "log.h"

//Check here
//https://dominoc925-pages.appspot.com/mapplets/cs_ecef.html
bool print_wgs84_coordinate(Wgs84Coordinate_t Wgs84Coordinate, bool one_line) {
    bool res = true;
    if(false == one_line) {
        io_printf("x: %f m" CRLF, Wgs84Coordinate.x_cm/100.0);
        io_printf("y: %f m" CRLF, Wgs84Coordinate.y_cm/100.0);
        io_printf("z: %f m" CRLF, Wgs84Coordinate.z_cm/100.0);
        io_printf("acc: %f m" CRLF, Wgs84Coordinate.acc_cm/100.0);
    }
    io_printf("XYZ %f %f %f m Acc:%f m" CRLF,
              Wgs84Coordinate.x_cm/100.0,
              Wgs84Coordinate.y_cm/100.0,
              Wgs84Coordinate.z_cm/100.0,
              Wgs84Coordinate.acc_cm/100.0 );

    return res;
}

bool print_coordinate(GnssCoordinate_t coordinate, bool one_line) {
    bool res = true;
    if(false == one_line) {
        io_printf("latitude : %f " CRLF, coordinate.latitude);
        io_printf("longitude: %f " CRLF, coordinate.longitude);
    }
    io_printf(" %f %f" CRLF, coordinate.latitude, coordinate.longitude);

    return res;
}

char* coordinate2str(void* value) {
    static char str[100] = "";
    GnssCoordinate_t dot={0,0};
    memcpy(&dot,value,sizeof(GnssCoordinate_t));
    if(value) {
        snprintf(str, sizeof(str), "%f %f", dot.latitude, dot.longitude);
    }
    return str;
}

bool print_velocity(GnssVelocity_t velocity) {
    bool res = true;
    io_printf("v_north: %f cm/s" CRLF, velocity.velocity_north);
    io_printf("v_east: %f cm/s" CRLF, velocity.velocity_east);
    io_printf("v_down: %f cm/s" CRLF, velocity.velocity_down);
    io_printf("speed: %f cm/s" CRLF, velocity.speed);

    io_printf("speed_ground: %f cm/s" CRLF, velocity.speed_ground);
    io_printf("speed: %f +/-%f cm/s" CRLF, velocity.speed, velocity.accuracy_speed);
    io_printf("course: %f +/-%f" CRLF, velocity.heading, velocity.accuracy_course);
    return res;
}

