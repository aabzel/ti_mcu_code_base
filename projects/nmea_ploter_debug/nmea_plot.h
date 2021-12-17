#ifndef PLOT_NMEA_LOG_H
#define PLOT_NMEA_LOG_H

#include <stdbool.h>
#include <stdint.h>

bool nmea_init(void);
bool plot_nmea(char* file_name, char* out_file_name);

#endif /* PLOT_NMEA_LOG_H  */