#include "adc_commands.h"

#include <aux_adc.h>
#include <inttypes.h>

#include "adc_drv.h"
#include "convert.h"
#include "data_utils.h"
#include "io_utils.h"
#include "log.h"
#include "table_utils.h"

bool adc_trig_command(int32_t argc, char* argv[]) {
    AUXADCGenManualTrigger();
    return false;
}

bool adc_start_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(0 == argc) {
        res = true;
        // AUXADCEnableSync
        AUXADCEnableAsync(AUXADC_REF_FIXED, AUXADC_TRIGGER_MANUAL);
    } else if(2 == argc) {
        res = true;
        uint32_t refSource = 0;
        uint32_t trigger = 0;
        if(res) {
            res = try_str2uint32(argv[0], &refSource);
            if(false == res) {
                LOG_ERROR(NVS, "Unable to parse refSource %s", argv[0]);
            }
        }
        if(res) {
            res = try_str2uint32(argv[1], &trigger);
            if(false == res) {
                LOG_ERROR(NVS, "Unable to parse trigger %s", argv[1]);
            }
        }
        if(res) {
            AUXADCEnableAsync(refSource, trigger);
        }
    } else {
        LOG_ERROR(ADC, "Usage: ast refSource trigger");
        LOG_INFO(ADC, "in [0..]");
    }
    return res;
}

bool adc_wait_fifo_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(0 == argc) {
        res = true;
        uint32_t val = AUXADCReadFifo();
        io_printf("val: 0x%08x %u" CRLF, val, val);
    } else {
        LOG_ERROR(ADC, "Usage: awf");
    }
    return res;
}

static char* adc_stat2str(uint32_t stat) {
    char* name = "no";
    switch(stat) {
    case AUXADC_FIFO_EMPTY_M:
        name = "EMPTY";
        break;
    case AUXADC_FIFO_ALMOST_FULL_M:
        name = "ALMOST_FULL";
        break;
    case AUXADC_FIFO_FULL_M:
        name = "FULL";
        break;
    case AUXADC_FIFO_UNDERFLOW_M:
        name = "UNDERFLOW";
        break;
    case AUXADC_FIFO_OVERFLOW_M:
        name = "OVERFLOW";
        break;
    default:
        name = "undef";
        break;
    }

    return name;
}

bool adc_diag_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(0 == argc) {
        res = true;
        uint32_t stat;
        int32_t gain ;
        stat = AUXADCGetFifoStatus();
        uint32_t vap = AUXADCPopFifo();
        gain = AUXADCGetAdjustmentGain(  AUXADC_REF_FIXED);
        io_printf("fixed gain: %u" CRLF, gain);
        gain = AUXADCGetAdjustmentGain(  AUXADC_REF_VDDS_REL);
        io_printf("VDD gain: %u" CRLF, gain);
        io_printf("pop: 0x%08x %u" CRLF, vap, vap);
        io_printf("stat: 0x%08x %s" CRLF, stat, adc_stat2str(stat));
        io_printf("0: %u" CRLF, adcValue0);
    } else {
        LOG_ERROR(ADC, "Usage: ain");
    }
    return res;
}

bool adc_sel_in_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(0 == argc) {
        AUXADCSelectInput(ADC_COMPB_IN_VDDS);
    } else if(1 == argc) {
        res = true;
        uint32_t input = 0;
        if(res) {
            res = try_str2uint32(argv[0], &input);
            if(false == res) {
                LOG_ERROR(NVS, "Unable to parse input %s", argv[0]);
            }
        }
        if(res) {
            AUXADCSelectInput(input);
        }
    } else {
        LOG_ERROR(ADC, "Usage: asl in");
        LOG_INFO(ADC, "in [0..]");
    }
    return res;
}

bool adc_all_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(0 == argc) {
        res = true;
        int32_t  microvolts;
        uint32_t adc_value_pop=0,adc_value = 0, ch=0;
        static const table_col_t cols[] = {
            {7, "input"}, {10, "popValue"}, {10, "getValue"}, {11, "microvolts"} , {11, "volts"},
        };

        table_header(&dbg_o.s, cols, ARRAY_SIZE(cols));
        for(ch = 0; ch < ARRAY_SIZE(AdcChannelLUT); ch++) {
            AUXADCSelectInput(AdcChannelLUT[ch]);
            AUXADCEnableAsync(AUXADC_REF_FIXED, AUXADC_TRIGGER_MANUAL);
            AUXADCGenManualTrigger();
            //adc_value_pop = AUXADCPopFifo( );
            adc_value = AUXADCReadFifo();
            microvolts = AUXADCValueToMicrovolts(  AUXADC_FIXED_REF_VOLTAGE_NORMAL, adc_value);
            io_printf(TSEP"   %2u  "TSEP , AdcChannelLUT[ch]);
            io_printf(" %8u " TSEP , adc_value_pop);
            io_printf(" %8u " TSEP , adc_value);
            io_printf(" %9u "TSEP , microvolts);
            io_printf(" %f " TSEP , 0.000001f*((float)microvolts));
            io_printf(CRLF);
        }
        table_row_bottom(&dbg_o.s, cols, ARRAY_SIZE(cols));
    }
    return res;
}
