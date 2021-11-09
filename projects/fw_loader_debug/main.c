#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#include "boot_driver.h"
#include "byte_utils.h"
#include "convert.h"
#include "crc16_ccitt.h"
#include "data_utils.h"
#include "debug_info.h"
#include "io_utils.h"
#include "scan_serial_port.h"
#include "str_utils_ex.h"
#include "utils_file.h"
#include "macro_utils.h"
#include "memory_layout.h"

#define VERSION 1
#define FLASH_SIZE (352*K_BYTES)

#define MAX_BOOTLOADER_SIZE 0x1A000
#define MAX_GENERIC_SIZE (FLASH_SIZE-MAX_BOOTLOADER_SIZE-NVS_SIZE-K_BYTES)

static bool is_target_connected(void) {
    bool res = false;
    char txBuffer[100] = "";
    strncpy(txBuffer, "e 0" CRLF, sizeof(txBuffer));
    res = com_send_str(hComm, txBuffer, strlen(txBuffer));
    sleep(1);
    strncpy(txBuffer, "vi" CRLF, sizeof(txBuffer));
    res = com_send_str(hComm, txBuffer, strlen(txBuffer));

    sleep(1);
    char rxBuffer[200] = "";
    uint32_t real_rx_len = 0;
    res = com_receive_str(hComm, rxBuffer, sizeof(rxBuffer), &real_rx_len);
    if(0 < real_rx_len) {
        printf("\n[%s] len: %u", rxBuffer,real_rx_len);
        char *ptr=strstr(rxBuffer,"config:");
        if(ptr){
          printf("\n[d] Target connected!");
          res = true;
        }else{
            res = false;
        }
    }else{
        res = false;
    }
    return res;
}

static bool restore_target(void) {
    bool res = false;
    char txBuffer[100] = "";
    strncpy(txBuffer, "e 1" CRLF, sizeof(txBuffer));
    res = com_send_str(hComm, txBuffer, strlen(txBuffer));
    sleep(2);
    char rxBuffer[200] = "";
    uint32_t real_rx_len = 0;
    res = com_receive_str(hComm, rxBuffer, sizeof(rxBuffer), &real_rx_len);
    if(0 < real_rx_len) {
        printf("\n[%s]", rxBuffer);
        printf("\n[d] Target connected!");
        res = true;
    }
    return res;
}

static bool parse_fw_version(void) {
    bool res = false;
    char txBuffer[100] = "";
    strncpy(txBuffer, "vi" CRLF, sizeof(txBuffer));
    res = com_send_str(hComm, txBuffer, strlen(txBuffer));
    sleep(3);
    char rxBuffer[600] = "";
    uint32_t real_rx_len = 0;
    res = com_receive_str(hComm, rxBuffer, sizeof(rxBuffer), &real_rx_len);
    if(0 < real_rx_len) {
        printf("\n[%s]", rxBuffer);
        res = true;
    }
    return res;
}

bool parse_hex_array_after_prefix(char *prefix,char *rxBuffer,char *outArray, uint32_t out_arr_size){
    bool res = false;
    return res;
}

#define READ_PAUSE_S 0.3
static bool loader_flash_read(uint32_t addr,uint8_t *out_array, uint32_t len){
    bool res  = false;
    char txBuffer[100] = "";
#ifdef HAS_FW_UPDATE_DEBUG
    printf("\n[*] len: %u",len);
#endif
    snprintf(txBuffer,sizeof(txBuffer), "fr 0x%08x %u" CRLF, addr, len);
    res = com_send_str(hComm, txBuffer, strlen(txBuffer));
    sleep(READ_PAUSE_S);
    char rxBuffer[600] = "";
    uint32_t real_rx_len = 0;
    uint16_t crc16_read = 0;
    uint16_t crc16_calc = 0;
    res = com_receive_str(hComm, rxBuffer, sizeof(rxBuffer), &real_rx_len);
    if(0 < real_rx_len) {
#ifdef HAS_FW_UPDATE_DEBUG
        printf("\n[%s] len: %u", rxBuffer,real_rx_len);
#endif
        bool res_data = parse_lhex_array_after_prefix("0x",0,rxBuffer,out_array,len);
        if(false==res_data){
            printf("\n[e] parse data error");
        }
#ifdef HAS_FW_UPDATE_DEBUG
        print_mem(out_array, len,true,false,true,false);
#endif
        Type16Union_t un16 = {0};
        bool res_crc = parse_lhex_array_after_prefix("0x",1,rxBuffer,&un16.u8[0],2);
        if(false==res_crc){
            printf("\n[e] parse crc16 array error");
        }
        if (res_crc && res_data) {
            crc16_read = reverse_byte_order_uint16(un16.u16);
            crc16_calc= calc_crc16_ccitt_false(out_array,  len);
            if( crc16_calc==crc16_read) {
#ifdef HAS_FW_UPDATE_DEBUG
                printf("\n[*] CRC16 Ok");
#endif
                res = true;
            } else {
                printf("\n[e] CRC16 error read: 0x%04x calc: 0x%04x",crc16_read,crc16_calc);
                res  = false;
            }
        }
    }
    return res;
}

#define FLASH_RD_TRY_CNT 8U
#define FLASH_RD_CHANK_SIZE 256U
#define DFLT_APP_END_ADDR (DFLT_APP_START_ADDR+MAX_GENERIC_SIZE)
static bool read_current_firmware(void){
    bool res = false;
    uint32_t addr=DFLT_APP_START_ADDR;
    uint8_t flashChank[FLASH_RD_CHANK_SIZE]={0};
    uint8_t  try_num = 0 ;
    uint32_t chunk_num = 0;
    printf("\n[i] Load firmware from flash to RAM...");
    for(addr = DFLT_APP_START_ADDR, chunk_num=0;
        addr <= (DFLT_APP_END_ADDR-FLASH_RD_CHANK_SIZE);
        addr += FLASH_RD_CHANK_SIZE,chunk_num++){
        for(try_num=1 ;try_num<=FLASH_RD_TRY_CNT; try_num++){
            res = loader_flash_read(addr, flashChank, FLASH_RD_CHANK_SIZE);
            if (res) {
#ifdef HAS_FW_UPDATE_DEBUG
                printf("\n[*] read ok:");
#endif
                try_num = FLASH_RD_TRY_CNT*2;
            } else {
                printf("\n[w] read flash chunk error. chunk num: %u", chunk_num);
                if(FLASH_RD_TRY_CNT==try_num){
                    try_num = FLASH_RD_TRY_CNT*2;
                    addr = DFLT_APP_END_ADDR+1;
                    res = false;
                    printf("\n[e] read flash chunk error after %u try. chunk num: %u",FLASH_RD_TRY_CNT, chunk_num);
                }
            }
        }
    }
    return res;
}

int main(int argc, char* argv[]) {
    int ret = 0;
    bool target_connected = false;
    bool res = false;
    printf("\n[*] Firmware loader:");
    printf("\n[*] Version: %u", VERSION);
    if(argc < 3) {
        printf("\n[e] Lack of firmware");
        ret = 1;
        res = false;
    } else {
        res = true;
    }

    uint32_t baud_rate = 0;
    if(res) {
        res = try_str2uint32(argv[2], &baud_rate);
        if(false == res) {
            printf("\n[e] Unable to extract baud_rate %s", argv[2]);
        }
    }

    if(res) {
        res = init_serial(argv[1], baud_rate);
    }

    if(res) {
        target_connected = is_target_connected();
        if(target_connected){
            printf("\n[*] Spot target");
        }else{
            printf("\n[e] Target Disconnected");
        }
    }

    if(res) {
        res = parse_fw_version();
    }


    if(res) {
        uint8_t* pFirmwareArray = NULL;
        uint32_t firmware_size = 0;
        res = file_load_to_array(argv[3], &pFirmwareArray, &firmware_size);
        if (res) {
            printf("\n[*] Load firmware from file to RAM ok");
        }else{
            printf("\n[e] Unable to Load firmware from file to RAM");
        }
    }


    if(target_connected){
        res= read_current_firmware();
        if(res){
            printf("\n[*] Load firmware from Flash to RAM ok");
        }else{
            printf("\n[e] Unable to Load firmware from flash to RAM");
        }
    }


    if(target_connected) {
        res = restore_target();
        if(false == res) {
            printf("\n[e] Unable to restore target");
        }
    }

    CloseHandle(hComm);
    return ret;
}
