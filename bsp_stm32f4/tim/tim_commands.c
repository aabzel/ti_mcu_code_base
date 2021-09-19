#include "tim_commands.h"


#include <inttypes.h>

#include "clocks.h"
#include "convert.h"
#include "data_utils.h"
#include "log.h"
#include "table_utils.h"
#include "tim_drv.h"

bool tim_diag_ll_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(0 == argc) {
        res = true;

    } else {
        LOG_ERROR(TIM, "Usage: tdl");
    }

    return res;
}

bool tim_diag_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(0 == argc) {
        res = true;

    } else {
        LOG_ERROR(TIM, "Usage: td");
    }

    return res;
}

bool tim_set_prescaler_command(int32_t argc, char* argv[]) {
    bool res = false;
    uint8_t tim_base_id = 0;
    uint8_t tim_inst = 0;
    uint8_t prescaler = 0;
    if(3 == argc) {
        res = true;
        if(true == res) {
            res = try_str2uint8(argv[0], &tim_base_id);
            if(false == res) {
                LOG_ERROR(SYS, "Unable to extract tim_base_id %s", argv[0]);
            }
        }

        if(true == res) {
            res = try_str2uint8(argv[1], &tim_inst);
            if(false == res) {
                LOG_ERROR(SYS, "Unable to extract tim_inst %s", argv[1]);
            }
            if(4 <= tim_inst) {
                res = false;
            }
        }

        if(true == res) {
            res = try_str2uint8(argv[2], &prescaler);
            if(false == res) {
                LOG_ERROR(SYS, "Unable to extract prescaler %s", argv[2]);
            }
        }
    } else {
        LOG_ERROR(TIM, "Usage: tsp base inst prescaler");
    }

    if(res) {
    }

    return res;
}

bool tim_set_laod_command(int32_t argc, char* argv[]) {
    bool res = false;
    uint8_t tim_base_id = 0;
    uint8_t tim_inst = 0;
    uint32_t load = 0;
    if(3 == argc) {
        res = true;
        if(true == res) {
            res = try_str2uint8(argv[0], &tim_base_id);
            if(false == res) {
                LOG_ERROR(SYS, "Unable to extract tim_base_id %s", argv[0]);
            }
        }

        if(true == res) {
            res = try_str2uint8(argv[1], &tim_inst);
            if(false == res) {
                LOG_ERROR(SYS, "Unable to extract tim_inst %s", argv[1]);
            }
            if(4 <= tim_inst) {
                res = false;
            }
        }

        if(true == res) {
            res = try_str2uint32(argv[2], &load);
            if(false == res) {
                LOG_ERROR(SYS, "Unable to extract load %s", argv[2]);
            }
        }
    } else {
        LOG_ERROR(TIM, "Usage: tsl base inst load");
    }

    if(res) {
    }

    return res;
}

bool tim_set_period_command(int32_t argc, char* argv[]) {
    bool res = false;
    uint8_t tim_index = 0;
    uint32_t pesiod_ms = 1000;
    if(2 == argc) {
        res = true;
        if(true == res) {
            res = try_str2uint8(argv[0], &tim_index);
            if(false == res) {
                LOG_ERROR(SYS, "Unable to extract index %s", argv[0]);
            }
            if(ARRAY_SIZE(TimerItem) <= tim_index) {
                res = false;
            }
        }

        if(true == res) {
            res = try_str2uint32(argv[1], &pesiod_ms);
            if(false == res) {
                LOG_ERROR(SYS, "Unable to extract pesiod_ms %s", argv[1]);
            }
        }
    } else {
        LOG_ERROR(TIM, "Usage: tspr tim_index pesiod_ms");
        res = false;
    }
    if(res) {
        uint32_t prescaler = 0;
        uint32_t load = 0;
        res = tim_calc_registers(pesiod_ms, SYS_FREQ, prescaler, &load, 0xFFFFFFFF);
        if(res) {
        }
    }
    return res;
}
