
#include "tcan4550_commands.h"

#include <stdio.h>

#include "convert.h"
#include "debug_info.h"
#include "io_utils.h"
#include "log.h"
#include "str_utils.h"
#include "tcan4550_drv.h"
#include "tcan4550_diag.h"
#include "table_utils.h"
#include "data_utils.h"

bool tcan4550_diag_command(int32_t argc, char* argv[]){
    bool res = false;
    uint32_t out_reg = 0;

    out_reg = 0;
    res = tcan4550_read_reg(ADDR_DEVICE_ID0, &out_reg);
    res=tcan4550_parse_reg_dev_id0(out_reg);

    out_reg = 0;
    res = tcan4550_read_reg(ADDR_DEVICE_ID1, &out_reg);
    res = tcan4550_parse_reg_dev_id1(out_reg);

    out_reg = 0;
    res = tcan4550_read_reg(ADDR_SPI_2_REV, &out_reg);
    res = tcan4550_parse_reg_revision(out_reg);

    out_reg = 0;
    res = tcan4550_read_reg(ADDR_STATUS, &out_reg);
    res = tcan4550_parse_reg_status(out_reg);

    out_reg = 0;
    res = tcan4550_read_reg(ADDR_DEV_CONFIG, &out_reg);
    res = tcan4550_parse_reg_mode_op_cfg(out_reg);

    out_reg = 0;
    res = tcan4550_read_reg(ADDR_IF, &out_reg);
    res =  tcan4550_parse_reg_interrupt_flags(  out_reg);
    return res;
}

bool tcan4550_init_command(int32_t argc, char* argv[]){
    bool res = false;
    res=tcan4550_init();
    return res;
}

bool tcan4550_int_diag_command(int32_t argc, char* argv[]){
    bool res = false;
    return res;
}

bool tcan4550_read_reg_command(int32_t argc, char* argv[]){
    bool res = false;
    uint16_t address=0;
    uint32_t reg_val=0;
    if(1==argc){
        res = try_str2uint16(argv[0], &address);
        if(false == res) {
            LOG_ERROR(CAN, "Unable to extract address %s", argv[0]);
        }
    } else {
        LOG_ERROR(CAN, "Usage: crr addr");
    }
    if(true == res) {
        res = tcan4550_read_reg( address, &reg_val);
        if (res) {
            io_printf("0x%08x 0b%s"CRLF, reg_val, utoa_bin32(reg_val));
        }else{
            LOG_ERROR(CAN, "read error");
        }
    }
    return res;
}

bool tcan4550_read_command(int32_t argc, char* argv[]){
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
        LOG_ERROR(CAN, "Usage: cr addr len");
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

static bool tcan4550_reg_map_diag(char* key_word1, char* key_word2){
    bool res = false;
    uint16_t reg_cnt = tcan4550_get_reg_cnt();
    uint32_t reg_val=0;
    static const table_col_t cols[] = {{5, "Num"},
                                       {11, "Name"},
                                       {8, "addr"},
                                       {12, "value,hex"},
                                       {41, "reg value,bin"}};
    table_header(&dbg_o.s, cols, ARRAY_SIZE(cols));
    uint16_t num = 0,i=0;
    char temp_str[200];
    for(i=0;i<reg_cnt;i++){
        res = tcan4550_read_reg(  tCan4550RegLUT[i].addr, &reg_val);
        if(res){
          strcpy(temp_str, TSEP);
          snprintf(temp_str, sizeof(temp_str), "%s %9s " TSEP, temp_str, tCan4550RegLUT[i].name);
          snprintf(temp_str, sizeof(temp_str), "%s 0x%04x " TSEP, temp_str, tCan4550RegLUT[i].addr);
          snprintf(temp_str, sizeof(temp_str), "%s 0x%08x " TSEP, temp_str, reg_val);
          snprintf(temp_str, sizeof(temp_str), "%s %s " TSEP, temp_str, utoa_bin32(reg_val));
          snprintf(temp_str, sizeof(temp_str), "%s" CRLF, temp_str);
          if(is_contain(temp_str, key_word1, key_word2)) {
              io_printf(TSEP " %3u ", num);
              io_printf("%s", temp_str);
              num++;
          }
        }
    }

    table_row_bottom(&dbg_o.s, cols, ARRAY_SIZE(cols));
    return res;
}

bool tcan4550_reg_map_command(int32_t argc, char* argv[]){
    bool res = false;
    char keyWord1[20] = "";
    char keyWord2[20] = "";
    if(0 <= argc) {
        strncpy(keyWord1, "", sizeof(keyWord1));
        strncpy(keyWord2, "", sizeof(keyWord2));
        res = true;
    }
    if(1 <= argc) {
        strncpy(keyWord1, argv[0], sizeof(keyWord1));
        res = true;
    }
    if(2 <= argc) {
        strncpy(keyWord2, argv[1], sizeof(keyWord2));
        res = true;
    }

    if(2 < argc) {
        LOG_ERROR(SYS, "Usage: cm keyWord1 keyWord2");
    }
    if(true == res) {
        res = tcan4550_reg_map_diag(keyWord1, keyWord2);
    }
    return res;
}

bool tcan4550_reset_command(int32_t argc, char* argv[]){
    bool res = false;
    res = tcan4550_reset();
    return res;
}

bool tcan4550_write_reg_command(int32_t argc, char* argv[]){
    bool res = false;
    uint16_t address=0;
    uint32_t reg_val = 0;
    if(1<=argc){
        res = try_str2uint16(argv[0], &address);
        if(false == res) {
            LOG_ERROR(CAN, "Unable to extract address %s", argv[0]);
        }
    }
    if(2<=argc){
        res = try_str2uint32(argv[1], &reg_val);
        if(false == res) {
            LOG_ERROR(CAN, "Unable to extract reg_val %s", argv[1]);
        }
    }

    if ((true==res) && (2==argc)) {
        res = tcan4550_write_reg(address, reg_val);
        if (true==res) {
            LOG_INFO(CAN, "write addr 0x%x val 0x%x ok",address,reg_val);
        }else{
            LOG_ERROR(CAN, "write err");
        }
    } else {
        LOG_ERROR(CAN, "Usage: cwr addr reg");
    }
    return res;
}

bool tcan4550_clear_mram_command(int32_t argc, char* argv[]){
    bool res = false;
    res = tcan4550_clear_mram();
    return res;
}


bool tcan4550_send_frame_command(int32_t argc, char* argv[]){
    bool res = false;
    uint16_t id= 0;
    uint64_t data64 = 0;
    if(1<=argc){
        res = try_str2uint16(argv[0], &id);
        if(false == res) {
            LOG_ERROR(CAN, "Unable to extract id %s", argv[0]);
        }
    }
    if(2<=argc){
        res = try_str2uint64(argv[1], &data64);
        if(false == res) {
            LOG_ERROR(CAN, "Unable to extract data %s", argv[1]);
        }
    }

    if (res) {
        res = tcan4550_send(id, data64);
        if (res) {
            LOG_INFO(CAN, "send id: 0x%06x data: 0x%llx ok", id, data64);
        } else {
            LOG_ERROR(CAN, "Unable to send frame");
        }
    }
    return res;
}
