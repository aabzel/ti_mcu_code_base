#include "health_monitor.h"

#include <stdbool.h>
#include <string.h>

#ifdef HAS_ADC
#include "adc_drv.h"
#endif
#include "cli_manager.h"
#include "core_driver.h"
#include "gnss_diag.h"
#include "gnss_utils.h"
#include "log.h"
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
#include "uart_common.h"
#include "uart_drv.h"
HealthMon_t HealthMon = {0};

bool health_monotor_init(void) {
    bool res = true;
    memset(&HealthMon, 0x00, sizeof(HealthMon));

    return res;
}

#define MIM_LORA_THROUGHPUT_BYTE_S 600

bool health_monotor_proc(void) {
    bool res = false;
#ifdef HAS_ADC
    float vKl30 = 0.0;
    vKl30 = adc_get_value_by_dio(DIO_KL30_ADC, true);
    if(vKl30 < KL30_UNDERVOL_ERRPR_THRESHOLD_V) {
        LOG_ERROR(HMOM, "vKl30 %f too low", vKl30);
        res = false;
    } else {
        if(vKl30 < KL30_UNDERVOL_WARNING_THRESHOLD_V) {
            LOG_WARNING(HMOM, "vKl30 %f low", vKl30);
        }
        res = true;
    }
#endif /*HAS_ADC*/

#ifdef HAS_SX1262
    if((Sx1262Instance.bit_rate / 8) < MIM_LORA_THROUGHPUT_BYTE_S) {
        LOG_ERROR(HMOM, "LoRaByteRate too low %f byte/s", Sx1262Instance.bit_rate / 8);
        res = false;
    }
#endif

    if(HealthMon.init_error){
        LOG_ERROR(HMOM, "InitError");
    }

    if(LoRaInterface.tx_err_cnt){
        LOG_ERROR(HMOM, "LoRaRxError");
    }
    if(true != cli_init_done) {
        cli_init_done = true;
    }

    float stack_precent = stack_used();
    if(50.0<stack_precent ){
        LOG_WARNING(HMOM, "StackUsed:%f %%", stack_precent);
        if(75.0<stack_precent ){
            LOG_ERROR(HMOM, "StackUsed:%f %%", stack_precent);
        }
    }
    if(0 < huart[UART_NUM_CLI].error_cnt){
        LOG_ERROR(UART, "Error");
        //res = init_uart_ll(UART_NUM_CLI, "CLI");
        //if(false==res){
       // }
    }

#if defined(HAS_CHECK_TIME) && defined(HAS_NMEA) && defined(HAS_UBLOX)
    bool res_eq = is_time_date_equal(&NavInfo.date_time, &NmeaData.time_date);

    bool res_nm = is_valid_time_date(&NmeaData.time_date);
    bool res_ub = is_valid_time_date(&NavInfo.date_time);
    if((false == res_eq) && res_nm && res_nm) {
        LOG_ERROR(HMOM, "Nmea and UBX Time different");
        LOG_INFO(HMOM, "Nmea:");
        print_time_date(&NmeaData.time_date);
        LOG_INFO(HMOM, "UBX:");
        print_time_date(&NavInfo.date_time);
    }
#endif
    return res;
}
