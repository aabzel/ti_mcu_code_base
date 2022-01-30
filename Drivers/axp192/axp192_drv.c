#include "axp192_drv.h"

#include <stdbool.h>
#include <stdint.h>

#include "log.h"
#include "i2c_drv.h"
#include "sys_config.h"

//https://github.com/bitengineers/esp32_axp192/blob/main/include/axp192.h
//https://github.com/bitengineers/esp32_axp192/blob/main/axp192.c


bool axp192_init(void) {
    bool res = true;
    return res;
}

bool axp192_process(void) {
    bool res = false;
    return res;
}

bool axp192_i2c_write(uint8_t reg, uint8_t value){
    bool res = false;
    bool out_res = true;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    if(cmd){
        esp_err_t  ret=0;
        ret= i2c_master_start(cmd);
        if(ESP_OK!=ret){
            LOG_ERROR(PMIC, "Err L:%u",__LINE__);
            out_res = false;
        }
        ret=i2c_master_write_byte(cmd, AXP192_I2C_ADDR | I2C_MASTER_WRITE, true);
        if(ESP_OK!=ret){
            LOG_ERROR(PMIC, "Err L:%u",__LINE__);
              out_res = false;
          }
        ret=i2c_master_write_byte(cmd, reg, true);
        if(ESP_OK!=ret){
            LOG_ERROR(PMIC, "Err L:%u",__LINE__);
              out_res = false;
          }
        ret=i2c_master_write_byte(cmd, value, true);
        if(ESP_OK!=ret){
            LOG_ERROR(PMIC, "Err L:%u",__LINE__);
              out_res = false;
          }
        ret=i2c_master_stop(cmd);
        if(ESP_OK!=ret){
            LOG_ERROR(PMIC, "Err L:%u",__LINE__);
              out_res = false;
          }
        ret=i2c_master_cmd_begin(AXP192_I2C, cmd, pdMS_TO_TICKS(10));
        if(ESP_OK!=ret){
            LOG_ERROR(PMIC, "Err L:%u",__LINE__);
              out_res = false;
          }
        i2c_cmd_link_delete(cmd);
        res= true;
    }
    return out_res ;
}

bool axp192_i2c_read(uint8_t reg, uint8_t *value){
    bool res = false;
    // i2w 0 0x34 0x23
    // i2r 0 0x34 1
    res=i2c_write(AXP192_I2C,  AXP192_I2C_ADDR, &reg, 1);
    if(res){
        res=i2c_read(AXP192_I2C, AXP192_I2C_ADDR, value, 1);
    }

    return res;
}
