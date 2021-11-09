#include "adc_commands.h"

#include <aux_adc.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>

#include "adc_drv.h"
#include "convert.h"
#include "data_utils.h"
#include "gpio_drv.h"
#include "io_utils.h"
#include "log.h"
#include "table_utils.h"
#include "writer_config.h"

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

bool adc_init_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(0 == argc) {
        res = adc_init();
        if(false == res) {
            LOG_ERROR(ADC, "ADC init error");
        } else {
            LOG_INFO(ADC, "ADC init OK");
        }
    } else {
        LOG_ERROR(ADC, "Usage: ait");
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
    static char name[40] = "";
    memset(name, 0x00, sizeof(name));
    if(AUXADC_FIFO_EMPTY_M == (stat & AUXADC_FIFO_EMPTY_M)) {
        snprintf(name, sizeof(name), "%s empty", name);
    }
    if(AUXADC_FIFO_FULL_M == (stat & AUXADC_FIFO_FULL_M)) {
        snprintf(name, sizeof(name), "%s full", name);
    }
    if(AUXADC_FIFO_UNDERFLOW_M == (stat & AUXADC_FIFO_UNDERFLOW_M)) {
        snprintf(name, sizeof(name), "%s underflow", name);
    }
    if(AUXADC_FIFO_OVERFLOW_M == (stat & AUXADC_FIFO_OVERFLOW_M)) {
        snprintf(name, sizeof(name), "%s overflow", name);
    }

    return name;
}

bool adc_diag_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(0 == argc) {
        res = true;
        uint32_t stat;
        int32_t gain;
        io_printf("ChipId: %u" CRLF, HapiGetChipId());
        stat = AUXADCGetFifoStatus();
        // uint32_t vap = AUXADCPopFifo();
        gain = AUXADCGetAdjustmentGain(AUXADC_REF_FIXED);
        io_printf("fixed gain: %u" CRLF, gain);
        gain = AUXADCGetAdjustmentGain(AUXADC_REF_VDDS_REL);
        io_printf("VDD gain: %u" CRLF, gain);
        // io_printf("pop: 0x%08x %u" CRLF, vap, vap);
        io_printf("stat: 0x%08x %s" CRLF, stat, adc_stat2str(stat));
    } else {
        LOG_ERROR(ADC, "Usage: ad");
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
        int32_t microvolts = 0;
        uint32_t adc_value_pop = 0, i = 0;
        static const table_col_t cols[] = {{7, "input"},     {5, "DIO"},       {5, "pin"},
                                           {10, "popValue"}, {10, "getValue"}, {11, "microvolts"},
                                           {11, "volts"},    {11, "Scaled"},   {7, "name"}};

        table_header(&(curWriterPtr->s), cols, ARRAY_SIZE(cols));
        for(i = 0; i < ARRAY_SIZE(AdcItemsLUT); i++) {
            if(0xFF != AdcItemsLUT[i].adc_channel) {
                microvolts = AUXADCValueToMicrovolts(AUXADC_FIXED_REF_VOLTAGE_NORMAL, AdcCodes[i]);
                io_printf(TSEP "   %2u  " TSEP, AdcItemsLUT[i].adc_channel);
                io_printf("  %2u " TSEP, AdcItemsLUT[i].io_pin);
                io_printf(" %3u " TSEP, AdcItemsLUT[i].pin);
                io_printf(" %8u " TSEP, adc_value_pop);
                io_printf(" %8u " TSEP, AdcCodes[i]);
                io_printf(" %9u " TSEP, microvolts);
                io_printf("  %7.3f  " TSEP, 0.000001f * ((float)microvolts));
                io_printf("  %7.3f  " TSEP, 0.000001f * ((float)microvolts) * AdcItemsLUT[i].scale);
                io_printf("  %4s " TSEP, gpio_get_name(AdcItemsLUT[i].io_pin));
                io_printf(CRLF);
            }
        }
        table_row_bottom(&(curWriterPtr->s), cols, ARRAY_SIZE(cols));
    } else {
        LOG_ERROR(ADC, "Usage: ain");
    }
    return res;
}
