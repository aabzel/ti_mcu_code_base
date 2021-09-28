
#include "bq25171_q1_drv.h"

#include <gpio.h>
#include <stdbool.h>
#include <string.h>

#include "gpio_drv.h"
#include "sys_config.h"

bool bq25171_q1_init(void){
    bool res = false;
    res = bq25171_q1_charge_enable(false);
    res = gpio_set_pull_mode(  DIO_GNSS_INT, PULL_AIR) && res;
    res = gpio_set_dir( DIO_GNSS_INT, GPIO_DIR_NONE) && res;
    return res;
}

bool bq25171_q1_charge_enable(bool state){
    if(true==state){
        /*Active Low Charge Enable pin*/
        GPIO_writeDio(DIO_GNSS_INT, 0);
    } else if(false==state){
        GPIO_writeDio(DIO_GNSS_INT, 1);
    }
    return true;
}

ChargingStatus_t bq25171_q1_get_status(void){
    ChargingStatus_t ch_stat=CHRG_STAT_UNDEF;
    uint8_t temp=0;
    uint32_t stat1 = GPIO_readDio((uint32_t)DIO_BATT_SCL);
    uint32_t stat2 = GPIO_readDio((uint32_t)DIO_BATT_SDA);
    if (stat1) {
        temp |=(1<<0);
    }
    if (stat2) {
        temp |=(1<<1);
    }
    ch_stat =(ChargingStatus_t) temp;
    return ch_stat;
}
