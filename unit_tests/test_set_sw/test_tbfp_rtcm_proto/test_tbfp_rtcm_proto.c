#include "test_tbfp_rtcm_proto.h"

#ifdef X86_64
#include <stdio.h>
#endif

#include "fifo_char.h"
#include "lora_drv.h"
#include "log.h"
#include "test_rtcm3_proto.h"
#include "tbfp_protocol.h"
#include "rtcm3_protocol.h"
#include "system.h"
#include "unit_test_check.h"

bool test_tbfp_tunnel(void){
    LOG_DEBUG(TBFP,"%s():", __FUNCTION__);
    uint8_t BigRtcm3Message[350]={};
    uint32_t i=0;
    EXPECT_TRUE(tbfp_protocol_init(&TbfpProtocol[IF_LORA], IF_LORA,0xA5) ) ;
    EXPECT_TRUE(tbfp_protocol_init(&TbfpProtocol[IF_SX1262], IF_SX1262,0x1E));
    EXPECT_TRUE(fifo_init(&LoRaInterface.FiFoLoRaCharTx, &LoRaTxBuff[0], sizeof(LoRaTxBuff)));

    for(i=0;i<11;i++){
        EXPECT_TRUE(tbfp_send(BigRtcm3Message, sizeof(BigRtcm3Message), IF_LORA, 0));
        uint32_t count =0;
        do{
            lora_transmit_from_queue(10,0, 1);
            count = fifo_get_count(&LoRaInterface.FiFoLoRaCharTx);
        }while(0<count );
    }

    EXPECT_EQ(11,TbfpProtocol[IF_LORA].rx_pkt_cnt);
    return true;
}

bool test_tbfp_rtcm_path(void) {
#ifdef X86_64
    LOG_DEBUG(TBFP,"%s():", __FUNCTION__);
#endif
    uint32_t i=0;
    EXPECT_TRUE( set_log_level(TBFP , LOG_LEVEL_INFO ));
    EXPECT_TRUE( set_log_level(RTCM, LOG_LEVEL_INFO ));
    EXPECT_TRUE(tbfp_protocol_init(&TbfpProtocol[IF_LOOPBACK], IF_LOOPBACK,0xA5));
    EXPECT_TRUE(rtcm3_protocol_init(&Rtcm3Protocol[IF_LOOPBACK], IF_LOOPBACK, false));
    for (i=1; i<1000;i++) {
        EXPECT_TRUE_NUM(tbfp_send((uint8_t* )rtcm3_message, sizeof(rtcm3_message), IF_LOOPBACK, 0),i);
        EXPECT_EQ(i,Rtcm3Protocol[IF_LOOPBACK].rx_pkt_cnt);
    }

    return true;
}

