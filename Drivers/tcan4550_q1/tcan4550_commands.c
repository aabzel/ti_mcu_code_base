
#include "tcan4550_commands.h"

#include "convert.h"
#include "debug_info.h"
#include "tcan4550_drv.h"
#include "log.h"

bool tcan4550_diag_command(int32_t argc, char* argv[]){
    bool res = false;
    return res;
}

bool tcan4550_init_command(int32_t argc, char* argv[]){
    bool res = false;
    return res;
}

bool tcan4550_int_diag_command(int32_t argc, char* argv[]){
    bool res = false;
    return res;
}

bool tcan4550_read_reg_command(int32_t argc, char* argv[]){
    bool res = false;
    uint16_t address=0;
    uint8_t len = 0;
    uint8_t data[32]="";
    memset(data, 0xFF, sizeof(data));
    if(2==argc){
        res = try_str2uint16(argv[0], &address);
        if(false == res) {
            LOG_ERROR(CAN, "Unable to extract address %s", argv[0]);
        }
        res = try_str2uint8(argv[1], &len);
        if(false == res) {
            LOG_ERROR(CAN, "Unable to extract len %s", argv[1]);
        }
    }else{
        LOG_ERROR(CAN, "Usage: crr addr len");
    }
    if(true == res) {
        res = tcan4550_read( address, len, data, sizeof(data));
        if(res){
            print_mem(data,sizeof(data),true,true);
        }else{
            LOG_ERROR(CAN, "read error");
        }
    }
    return res;
}

bool tcan4550_reg_map_command(int32_t argc, char* argv[]){
    bool res = false;
    return res;
}

bool tcan4550_reset_command(int32_t argc, char* argv[]){
    bool res = false;
    res = tcan4550_reset();
    return res;
}

bool tcan4550_write_reg_command(int32_t argc, char* argv[]){
    bool res = false;
    return res;
}
