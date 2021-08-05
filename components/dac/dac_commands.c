#include "dac_commands.h"

#include <aux_dac.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>

#include "bit_utils.h"
#include "convert.h"
#include "dac_drv.h"
#include "data_utils.h"
#include "io_utils.h"
#include "log.h"
#include "table_utils.h"

/*DAC_VOUT_SEL
 * see Figure 19-28. AUX Analog Block Diagram
 * */
static const char* code2outcon(uint8_t code) {
    const char* name = "undf";
    switch(code) {
    case 0:
        name = "nothing";
        break;
    case 1:
        name = "COMPB_REF";
        break;
    case 2:
        name = "COMPA_REF";
        break;
    case 4:
        name = "COMPA_IN";
        break;
    default:
        name = "error";
        break;
    }
    return name;
}

bool parse_dac_ctl_reg(uint32_t dac_ctl) {
    io_printf("%s" CRLF, code2outcon(MASK_3BIT & dac_ctl));
    io_printf("%s" CRLF, (1 << 3) == (dac_ctl & (1 << 3)) ? "1.28 V to 2.56 V." : "0 V to 1.28 V");
    io_printf("%s DAC buffer" CRLF, (1 << 4) == (dac_ctl & (1 << 4)) ? "Enable" : "Disable");
    io_printf("%s DAC" CRLF, (1 << 5) == (dac_ctl & (1 << 5)) ? "Enable" : "Disable");
    return true;
}

bool dac_diag_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(0 == argc) {
        res = true;
        uint32_t max_code = AUXDACCalcMax();
        uint32_t min_code = AUXDACCalcMin();
        io_printf("max %u" CRLF, max_code);
        io_printf("min %u" CRLF, min_code);
        uint8_t dac_val = HWREGB(AUX_ANAIF_BASE + AUX_ANAIF_O_DACVALUE);
        io_printf("dac_val 0x%x %u" CRLF, dac_val, dac_val);
        uint32_t dac_ctl = HWREG(AUX_ANAIF_BASE + AUX_ANAIF_O_DACCTL);
        parse_dac_ctl_reg(dac_ctl);
        uint8_t mux1_code= HWREGB(AUX_ADI4_BASE + ADI_4_AUX_O_MUX1);
        io_printf("MUX1 0x%x %u" CRLF, mux1_code, mux1_code);
    } else {
        LOG_ERROR(DAC, "Usage: dad");
    }
    return res;
}

bool dac_set_code_command(int32_t argc, char* argv[]) {
    bool res = false;
    uint8_t dac_code = 0;
    uint8_t dac_pin_id = 19;
    if(1 <= argc) {
        res = try_str2uint8(argv[0], &dac_code);
        if(false == res) {
            LOG_ERROR(DAC, "Unable to parse dac_code %s", argv[0]);
        }
    }
    if(2 <= argc) {
        res = try_str2uint8(argv[1], &dac_pin_id);
        if(false == res) {
            LOG_ERROR(DAC, "Unable to parse dac_code %s", argv[1]);
        }
    } else {
        LOG_ERROR(DAC, "Usage: dsc dac_code dac_pin_id");
        LOG_INFO(DAC, "dac_code");
        LOG_INFO(DAC, "dac_pin_id");
    }
    if(res) {
        AUXDACSetCode(dac_code);
        AUXDACEnable(io2comp(dac_pin_id));
    }

    return res;
}

bool dac_set_vol_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(2 == argc) {
        res = true;
        uint8_t dac_code = 0;
        uint8_t dac_pin_id = 0;
        float voltage = 0.0;
        if(res) {
            res = try_str2float(argv[0], &voltage);
            if(false == res) {
                LOG_ERROR(DAC, "Unable to parse voltage %s", argv[0]);
            }
        }
        if(res) {
            res = try_str2uint8(argv[1], &dac_pin_id);
            if(false == res) {
                LOG_ERROR(DAC, "Unable to parse dac_code %s", argv[1]);
            }
        }
        if(res) {
            uint32_t u_volt_out = voltage * 1000.0f;
            uint32_t max_code = AUXDACCalcMax();
            uint32_t min_code = AUXDACCalcMin();
            dac_code = AUXDACCalcCode(u_volt_out, min_code, max_code);
            io_printf("dac_code %u" CRLF, dac_code);
            AUXDACSetCode(dac_code);
            AUXDACEnable(io2comp(dac_pin_id));
        }
    } else {
        LOG_ERROR(DAC, "Usage: dsv voltage dac_pin_id");
        LOG_INFO(DAC, "dac_code");
    }
    return res;
}

bool dac_init_command(int32_t argc, char* argv[]) {
    bool res = dac_init();
    return res;
}

bool dac_set_mux_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(1 == argc) {
        res = true;
        uint8_t dac_pin_id;
        res = try_str2uint8(argv[0], &dac_pin_id);
        if(false == res) {
            LOG_ERROR(DAC, "Unable to parse dac_code %s", argv[0]);
        }
        if(res) {
            AUXDACEnable(io2comp(dac_pin_id));
        }
    } else {
        LOG_ERROR(DAC, "Usage: dsm dac_pin_id");
        LOG_INFO(DAC, "dac_pin_id");
    }
}
