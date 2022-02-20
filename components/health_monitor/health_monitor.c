#include "health_monitor.h"

#include <stdbool.h>
#include <string.h>
#ifdef HAS_ADC
#include "adc_drv.h"
#endif
#ifdef HAS_CLI
#include "cli_manager.h"
#endif
#include "core_utils.h"
#ifdef HAS_GNSS
#include "gnss_diag.h"
#include "gnss_utils.h"
#endif

#ifdef HAS_LOG
#include "log.h"
#endif

#ifdef HAS_RTCM3
#include "rtcm3_protocol.h"
#endif

#ifdef HAS_TBFP
#include "tbfp_protocol.h"
#endif

#ifdef HAS_LORA
#include "lora_drv.h"
#endif
#ifdef HAS_NMEA
#include "nmea_protocol.h"
#endif
#ifdef HAS_SX1262
#include "sx1262_drv.h"
#endif
#include "sys_config.h"
#ifdef HAS_UBLOX
#include "ublox_driver.h"
#endif
#ifdef HAS_UART
#include "uart_common.h"
#include "uart_drv.h"
#endif

#if 0
#if(TBFP_MAX_PAYLOAD < RTCM3_RX_MAX_FRAME_SIZE) /*Compile error*/
#error "TBFP frame must be big enough to store RTCM3 frame"
#endif
#endif

HealthMon_t HealthMon = {0};

bool health_monotor_init(void) {
    bool res = true;
    memset(&HealthMon, 0x00, sizeof(HealthMon));
#if defined(HAS_RTCM3) && defined(HAS_TBFP)
    if(TBFP_MAX_PAYLOAD < RTCM3_RX_MAX_FRAME_SIZE) {
        LOG_ERROR(HMON, "TBFPFrameMustBeBigEnoughToStoreRTCM3Frame TBFP:%u<RTCM3:%u", TBFP_MAX_PAYLOAD,
                  RTCM3_RX_MAX_FRAME_SIZE);
        res = false;
    } else {
        LOG_INFO(HMON, "TbfpRtcmFramesSizesOk RTCM3:%u<TBFP:%u", RTCM3_RX_MAX_FRAME_SIZE, TBFP_MAX_PAYLOAD);
    }
#endif
#ifdef HAS_RELAESE
    HealthMon.power = true;
#endif

#ifdef HAS_DEBUG
    HealthMon.power = false;
#endif
    return res;
}

#define MIM_LORA_THROUGHPUT_BYTE_S 1100

bool health_monotor_proc(void) {
    bool res = false;
#ifdef HAS_ADC
    if(HealthMon.power) {
        float vKl30 = 0.0;
        vKl30 = adc_get_value_by_dio(DIO_KL30_ADC, true);
        if(vKl30 < KL30_UNDERVOL_ERRPR_THRESHOLD_V) {
#ifdef HAS_LOG
            LOG_ERROR(HMON, "vKl30 %7.3f<%7.3f too low", vKl30, KL30_UNDERVOL_ERRPR_THRESHOLD_V);
#endif
            res = false;
        } else {
            if(vKl30 < KL30_UNDERVOL_WARNING_THRESHOLD_V) {
#ifdef HAS_LOG
                LOG_WARNING(HMON, "vKl30 %7.3f<%7.3f low", vKl30, KL30_UNDERVOL_WARNING_THRESHOLD_V);
#endif
            }
            res = true;
        }
    }
#endif /*HAS_ADC*/

#ifdef HAS_NMEA
    res = nmea_check();
#endif

#ifdef HAS_TBFP
    res = tbfp_check();
#endif

#ifdef HAS_RTCM3
    res = rtcm3_check();
#endif

#ifdef HAS_SX1262
#ifdef HAS_RTCM3
    res = rtcm3_check();
    if((Sx1262Instance.bit_rate / 8) < MIM_LORA_THROUGHPUT_BYTE_S) {
#ifdef HAS_LOG
        LOG_ERROR(HMON, "LoRaByteRate too low %f byte/s Need %u byte/s", Sx1262Instance.bit_rate / 8, MIM_LORA_THROUGHPUT_BYTE_S);
#endif
        res = false;
    }
#endif
#endif

    if(HealthMon.init_error) {
#ifdef HAS_LOG
        LOG_ERROR(HMON, "InitError");
#endif
    }

#ifdef HAS_LORA
    if(LoRaInterface.tx_err_cnt) {
#ifdef HAS_LOG
        LOG_DEBUG(HMON, "LoRaTxError %u", LoRaInterface.tx_err_cnt);
#endif
    }
#endif

#ifdef HAS_CLI
    if(true != cli_init_done) {
        cli_init_done = true;
    }
#endif

    float stack_precent = stack_used();
    if(50.0 < stack_precent) {
#ifdef HAS_LOG
        LOG_WARNING(HMON, "StackUsed:%f %%", stack_precent);
#endif
        if(75.0 < stack_precent) {
#ifdef HAS_LOG
            LOG_ERROR(HMON, "StackUsed:%f %%", stack_precent);
#endif
        }
    }
#ifdef HAS_UART
    if(0 < huart[UART_NUM_CLI].error_cnt) {
#ifdef HAS_LOG
        LOG_ERROR(UART, "Error");
#endif
        // res = init_uart_ll(UART_NUM_CLI, "CLI");
        // if(false==res){
        // }
    }
#endif

#ifdef HAS_RTCM3
    static uint32_t lora_lost_pkt_cnt_prev = 0;
    uint32_t lora_lost_pkt_cnt_diff = 0;
    lora_lost_pkt_cnt_diff = Rtcm3Protocol[IF_UART1].lost_pkt_cnt[IF_LORA] - lora_lost_pkt_cnt_prev;
    if(0 < lora_lost_pkt_cnt_diff) {
#ifdef HAS_LOG
        LOG_WARNING(HMON, "RTCM3 UART1 lost %u", lora_lost_pkt_cnt_diff);
#endif
    }
    lora_lost_pkt_cnt_prev = Rtcm3Protocol[IF_UART1].lost_pkt_cnt[IF_LORA];
#endif

#if defined(HAS_CHECK_TIME) && defined(HAS_NMEA) && defined(HAS_UBLOX)
    bool res_eq = is_time_date_equal(&NavInfo.date_time, &NmeaData.time_date);

    bool res_nm = is_valid_time_date(&NmeaData.time_date);
    bool res_ub = is_valid_time_date(&NavInfo.date_time);
    if((false == res_eq) && res_nm && res_nm) {
#ifdef HAS_LOG
        LOG_ERROR(HMON, "Nmea and UBX Time different");
        LOG_INFO(HMON, "Nmea:");
        print_time_date(&NmeaData.time_date);
        LOG_INFO(HMON, "UBX:");
        print_time_date(&NavInfo.date_time);
#endif
    }
#endif
    return res;
}
