#include "test_tbfp_proto.h"

#ifdef X86_64
#include <stdio.h>
#endif

#include "tbfp_protocol.h"
#include "unit_test_check.h"
#include "system.h"

static const uint8_t tbfp_message1[ ] = {0xA5,0x00,0x63};
static const uint8_t tbfp_message2[ ] = {0xA5,0x01,0x33,0xa9};

bool test_tbfp_proto_0(void) {
#ifdef X86_64
    printf("\n%s():", __FUNCTION__);
#endif
    uint32_t i=0;
    TbfpProtocol_t instance;
    EXPECT_TRUE( tbfp_protocol_init(&instance, IF_RS232));

    uint32_t init_pkt_cnt = instance.rx_pkt_cnt;
    for(i=0;i<sizeof(tbfp_message1);i++){
        printf("\n%s(): %u", __FUNCTION__, i);
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
        printf("\n%s(): %u", __FUNCTION__, i);
        EXPECT_TRUE( tbfp_proc_byte(&instance, tbfp_message2[i]));
    }
    EXPECT_EQ(init_pkt_cnt+1,instance.rx_pkt_cnt);
    return false;
}

bool test_tbfp_proto_1(void) {
#ifdef X86_64
    printf("\n%s():", __FUNCTION__);
#endif
    EXPECT_TRUE( is_tbfp_protocol(tbfp_message1, sizeof(tbfp_message1)));
    EXPECT_TRUE( is_tbfp_protocol(tbfp_message2, sizeof(tbfp_message2)));
    return false;
}

bool test_tbfp_types(void) {
#ifdef X86_64
    printf("\n%s():", __FUNCTION__);
#endif

    return false;
}
