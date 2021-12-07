#include "log.h"
#if 0
class TmpLogLevel {
    log_level_t saved_level;
    log_facility_t saved_facility;

public:
    TmpLogLevel(log_facility_t tmp_facility, log_level_t tmp_level) {
        saved_facility = tmp_facility;
        saved_level = get_log_level(saved_facility);
        set_log_level(saved_facility, tmp_level);
    }
    ~TmpLogLevel() {
        set_log_level(saved_facility, saved_level);
    }
};

#endif
