#include "test_tbfp_proto.h"

#ifdef X86_64
#include <stdio.h>
#endif

#include "tbfp_protocol.h"
#include "unit_test_check.h"
#include "system.h"

static const uint8_t tbfp_message1[ ] = {0xA5,0x00,0x63};
static const uint8_t tbfp_message2[ ] = {0xA5,0x01,0x33,0xa9};
static const uint8_t tbfp_message29[ ] = {0xA5, 0x1D,
                                          0x90,0x73,0xCB,0xCB,0x5E,0x8A,0xF8,0x00,
                                          0x00,0x80,0xAE,0xFE,0xFF,0x00,0x00,0x00,
                                          0x00,0x00,0x80,0x76,0x40,0x00,0x00,0x00,
                                          0x00,0x00,0x80,0x76,0x40,
                                          0x7B};

bool test_tbfp_proto_overlen(void){
	#ifdef X86_64
    printf("\n%s():", __FUNCTION__);
#endif
    uint32_t i=0;
    TbfpProtocol_t instance;
    EXPECT_TRUE( tbfp_protocol_init(&instance, IF_RS232));
	uint8_t tbfp_message[512] = {0};
	tbfp_message[0]=0xA5;
	tbfp_message[1]=0xFF; /*incorrect length*/
	for(i=0; i<sizeof(tbfp_message); i++) {
        EXPECT_TRUE( tbfp_proc_byte(&instance, tbfp_message[i]));
    }
	return false;
}

bool test_tbfp_proto_29(void) {
#ifdef X86_64
    printf("\n%s():", __FUNCTION__);
#endif
    uint32_t i=0;
    TbfpProtocol_t instance;
    EXPECT_TRUE( tbfp_protocol_init(&instance, IF_RS232));

    uint32_t init_pkt_cnt = instance.rx_pkt_cnt;
    for(i=0; i<sizeof(tbfp_message29); i++) {
#ifdef X86_64
        printf("\n%s(): %u", __FUNCTION__, i);
#endif
        EXPECT_TRUE( tbfp_proc_byte(&instance, tbfp_message29[i]));
    }
    EXPECT_EQ(init_pkt_cnt+1,instance.rx_pkt_cnt);

    for(i=0;i<sizeof(tbfp_message2);i++){
#ifdef X86_64
        printf("\n%s(): %u", __FUNCTION__, i);
#endif
        EXPECT_TRUE( tbfp_proc_byte(&instance, tbfp_message2[i]));
    }
    EXPECT_EQ(init_pkt_cnt+2,instance.rx_pkt_cnt);
    return false;
}

bool test_tbfp_proto_0(void) {
#ifdef X86_64
    printf("\n%s():", __FUNCTION__);
#endif
    uint32_t i=0;
    TbfpProtocol_t instance;
    EXPECT_TRUE( tbfp_protocol_init(&instance, IF_RS232));

    uint32_t init_pkt_cnt = instance.rx_pkt_cnt;
    for(i=0;i<sizeof(tbfp_message1);i++){
#ifdef X86_64
        printf("\n%s(): %u", __FUNCTION__, i);
#endif
        EXPECT_TRUE( tbfp_proc_byte(&instance, tbfp_message1[i]));
    }
    EXPECT_EQ(init_pkt_cnt+1,instance.rx_pkt_cnt);
    return false;
}

bool test_tbfp_proto_2(void) {
#ifdef X86_64
    printf("\n%s():", __FUNCTION__);
#endif
    uint32_t i=0;
    TbfpProtocol_t instance;
    EXPECT_TRUE( tbfp_protocol_init(&instance, IF_RS232));

    uint32_t init_pkt_cnt = instance.rx_pkt_cnt;
    for(i=0;i<sizeof(tbfp_message2);i++){
#ifdef X86_64
        printf("\n%s(): %u", __FUNCTION__, i);
#endif
        EXPECT_TRUE( tbfp_proc_byte(&instance, tbfp_message2[i]));
    }
    EXPECT_EQ(init_pkt_cnt+1,instance.rx_pkt_cnt);
    return false;
}

bool test_tbfp_proto_1(void) {
#ifdef X86_64
    printf("\n%s():", __FUNCTION__);
#endif
    EXPECT_TRUE( is_tbfp_protocol((uint8_t* )tbfp_message1, sizeof(tbfp_message1)));
    EXPECT_TRUE( is_tbfp_protocol((uint8_t* )tbfp_message2, sizeof(tbfp_message2)));
    return false;
}

bool test_tbfp_types(void) {
#ifdef X86_64
    printf("\n%s():", __FUNCTION__);
#endif

    return false;
}
