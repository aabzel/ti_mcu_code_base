#include "bmp180_commands.h"

#include <inttypes.h>

#include "bmp180_drv.h"
#include "convert.h"
#include "data_utils.h"
#include "debug_info.h"
#include "io_utils.h"
#include "i2c_drv.h"
#include "log.h"
 
bool bmp180_diag_command(int32_t argc, char* argv[]) { 
    return false;
}

//bmr 0x0D
bool bmp180_read_reg_command(int32_t argc, char* argv[]){
    bool res = false;
       if(1 == argc) {
           res = true;
           uint8_t reg_addr = 0;
           if(true == res) {
               res = try_str2uint8(argv[0], &reg_addr);
               if(false == res) {
                   LOG_ERROR(I2C, "Unable to parse reg_addr", argv[0]);
               }
           }

           if(true == res) {
               uint8_t reg_val=0xFF;
               res = bmp180_read_reg( reg_addr, &reg_val);
               if(false == res) {
                   LOG_ERROR(I2C, "Unable to read bmp180 reg");
               } else {

                   io_printf("reg: 0x%x"CRLF,reg_val);
               }
           }
       } else {
           LOG_ERROR(I2C, "Usage: bmr reg_addr");
           LOG_INFO(I2C, "reg_addr");
       }
       return res;
}
 
