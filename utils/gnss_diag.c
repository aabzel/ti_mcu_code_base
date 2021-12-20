#include "gnss_diag.h"
// http://traveleu.ru/map/GPSconverter.htm
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#include "gnss_utils.h"
#include "io_utils.h"
#include "log.h"


bool print_coordinate(GnssCoordinate_t coordinate, bool one_line) {
    bool res = true;
    if(false == one_line) {
        io_printf("latitude : %f " CRLF, coordinate.latitude);
        io_printf("longitude: %f " CRLF, coordinate.longitude);
    }
    io_printf(" %f %f" CRLF, coordinate.latitude, coordinate.longitude);

    return res;
}

char* coordinate2str(GnssCoordinate_t* value) {
    static char str[100] = "";
    if(value) {
        snprintf(str, sizeof(str), "%f %f", value->latitude, value->longitude);
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

