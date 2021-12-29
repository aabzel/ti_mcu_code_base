#ifndef ZED_F9P_DIAG_H
#define ZED_F9P_DIAG_H

#include <stdint.h>
char *RtkFixDur2str(uint32_t time_ms);
const char *rtk_mode2str(uint8_t code);
const char *rtk_channel2str(uint8_t code);
#endif /* ZED_F9P_DIAG_H  */
