#include "health_monitor.h"

#include <stdbool.h>
#include <string.h>

#ifdef HAS_ADC
#include "adc_drv.h"
#endif
#include "cli_manager.h"
#include "log.h"
#ifdef HAS_SX1262
#include "sx1262_drv.h"
#endif
#include "sys_config.h"

HealthMon_t HealthMon={0};

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
    if((Sx1262Instance.bit_rate/8)<MIM_LORA_THROUGHPUT_BYTE_S){
        LOG_ERROR(HMOM, "LoRaByteRate too low %f byte/s", Sx1262Instance.bit_rate/8);
          res = false;
    }
#endif

    if(false == cli_init_done) {
        cli_init_done = true;
    }
    return res;
}
