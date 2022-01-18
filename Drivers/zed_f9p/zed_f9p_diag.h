#ifndef ZED_F9P_DIAG_H
#define ZED_F9P_DIAG_H

#include <stdint.h>

#include "gnss_utils.h"

char *uint2str(uint32_t val);
char *GnssDot2str(GnssCoordinate_t coordinate);
char *mm2str(uint32_t base_acc_mm);
char *RtkFixDur2str(uint32_t time_ms);
const char *rtk_mode2str(uint8_t code);
const char *rtk_channel2str(uint8_t code);
char *Distance2str(double distance);
char *mSec2str(uint16_t m_sec);

#endif /* ZED_F9P_DIAG_H  */
