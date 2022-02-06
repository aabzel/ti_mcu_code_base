#include "test_tbfp_proto.h"

#ifdef X86_64
#include <stdio.h>
#endif

#include "log.h"
#include "system.h"
#include "tbfp_protocol.h"
#include "unit_test_check.h"

static const uint8_t tbfp_message1[] = {0xA5, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3d};
static const uint8_t tbfp_message2[] = {0xA5, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x7f};
static const uint8_t tbfp_message29[] = {0xA5, 0x00, 0x00, 0x00, 0x1D, 0x00, 0x90, 0x73, 0xCB, 0xCB, 0x5E, 0x8A,
                                         0xF8, 0x00, 0x00, 0x80, 0xAE, 0xFE, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00,
                                         0x80, 0x76, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x76, 0x40, 0x41};

bool test_tbfp_proto_0(void) {
    LOG_DEBUG(TBFP, "%s():", __FUNCTION__);

    uint32_t i = 0;
    TbfpProtocol_t instance;
    EXPECT_TRUE(tbfp_protocol_init(&instance, IF_RS232,0xA5));

    uint32_t init_pkt_cnt = instance.rx_pkt_cnt;
    for(i = 0; i < sizeof(tbfp_message1); i++) {
#ifdef X86_64
        printf("\n%s(): %u", __FUNCTION__, i);
#endif
        EXPECT_TRUE(tbfp_proc_byte(&instance, tbfp_message1[i]));
    }
    EXPECT_EQ(init_pkt_cnt + 1, instance.rx_pkt_cnt);
    return true;
}

bool test_tbfp_types(void) {

    LOG_DEBUG(TBFP, "%s():", __FUNCTION__);
    EXPECT_EQ(1, sizeof(TbfpFlags_t));
    EXPECT_EQ(6, sizeof(TbfHeader_t));

    return true;
}
bool test_tbfp_proto_1(void) {
    LOG_DEBUG(TBFP, "%s():", __FUNCTION__);
    EXPECT_TRUE(tbfp_protocol_init(&TbfpProtocol[IF_LOOPBACK], IF_LOOPBACK,0xA5));
    EXPECT_TRUE(is_tbfp_protocol((uint8_t*)tbfp_message1, sizeof(tbfp_message1),IF_LOOPBACK));
    EXPECT_TRUE(is_tbfp_protocol((uint8_t*)tbfp_message2, sizeof(tbfp_message2),IF_LOOPBACK));
    return true;
}

bool test_tbfp_proto_auto(void) {
    LOG_INFO(TBFP, "%s():", __FUNCTION__);
    uint8_t tbfp_generated[10] = {};
    memset(tbfp_generated, 0, sizeof(tbfp_generated));
    EXPECT_TRUE(tbfp_generate_frame(tbfp_generated, sizeof(tbfp_generated),IF_RS232));
    EXPECT_TRUE(is_tbfp_protocol((uint8_t*)tbfp_generated, sizeof(tbfp_generated),IF_RS232));
    return true;
}

bool test_tbfp_proto_2(void) {
    LOG_DEBUG(TBFP, "%s():", __FUNCTION__);
    uint32_t i = 0;
    TbfpProtocol_t instance;
    EXPECT_TRUE(tbfp_protocol_init(&instance, IF_RS232,0xA5));
    set_log_level(TBFP, LOG_LEVEL_DEBUG); /* uncommentin case of test fail*/
    uint32_t init_pkt_cnt = instance.rx_pkt_cnt;
    for(i = 0; i < sizeof(tbfp_message2); i++) {
        // printf("\n%s(): %u", __FUNCTION__, i);
        EXPECT_TRUE_NUM(tbfp_proc_byte(&instance, tbfp_message2[i]), i);
    }
    EXPECT_EQ(init_pkt_cnt + 1, instance.rx_pkt_cnt);
    return true;
}

bool test_tbfp_proto_overlen(void) {

    LOG_DEBUG(TBFP, "%s():", __FUNCTION__);
    uint32_t i = 0;
    TbfpProtocol_t instance;
    EXPECT_TRUE(tbfp_protocol_init(&instance, IF_RS232,0xA5));
    uint8_t tbfp_message[512] = {0};
    tbfp_message[0] = 0xA5;
    tbfp_message[1] = 0xFF; /*incorrect length*/
    for(i = 0; i < sizeof(tbfp_message); i++) {
        EXPECT_TRUE(tbfp_proc_byte(&instance, tbfp_message[i]));
    }
    return true;
}

bool test_tbfp_proto_29(void) {

    LOG_DEBUG(TBFP, "%s():", __FUNCTION__);

    uint32_t i = 0;
    TbfpProtocol_t instance;
    EXPECT_TRUE(tbfp_protocol_init(&instance, IF_RS232,0xA5));

    uint32_t init_pkt_cnt = instance.rx_pkt_cnt;
    for(i = 0; i < sizeof(tbfp_message29); i++) {
#ifdef X86_64
        printf("\n%s(): %u", __FUNCTION__, i);
#endif
        EXPECT_TRUE(tbfp_proc_byte(&instance, tbfp_message29[i]));
    }
    EXPECT_EQ(init_pkt_cnt + 1, instance.rx_pkt_cnt);

    for(i = 0; i < sizeof(tbfp_message2); i++) {
#ifdef X86_64
        printf("\n%s(): %u", __FUNCTION__, i);
#endif
        EXPECT_TRUE(tbfp_proc_byte(&instance, tbfp_message2[i]));
    }
    EXPECT_EQ(init_pkt_cnt + 2, instance.rx_pkt_cnt);
    return true;
}

bool test_tbfp_proto_flow_ctrl(void) {

    LOG_DEBUG(TBFP, "%s():", __FUNCTION__);

    EXPECT_TRUE(tbfp_protocol_init(&TbfpProtocol[IF_LOOPBACK], IF_LOOPBACK,0xA5));

    EXPECT_TRUE(tbfp_send_chat("12", 2, IF_LOOPBACK, 0));
    EXPECT_EQ(1, TbfpProtocol[IF_LOOPBACK].prev_s_num);

    EXPECT_TRUE(tbfp_send_chat("34", 2, IF_LOOPBACK, 0));
    EXPECT_EQ(2, TbfpProtocol[IF_LOOPBACK].prev_s_num);

    EXPECT_TRUE(tbfp_send_chat("56", 2, IF_LOOPBACK, 0));
    EXPECT_EQ(3, TbfpProtocol[IF_LOOPBACK].prev_s_num);
    EXPECT_EQ(4, TbfpProtocol[IF_LOOPBACK].s_num);
    EXPECT_EQ(3, TbfpProtocol[IF_LOOPBACK].con_flow);
    EXPECT_EQ(3, TbfpProtocol[IF_LOOPBACK].max_con_flow);
    return true;
}


bool test_tbfp_proto_flow_ctrl2(void) {
    LOG_DEBUG(TBFP, "%s():", __FUNCTION__);
    uint16_t prev_s_num=0;
    uint16_t max_flow=0;
    uint16_t con_flow=0;
    set_log_level(TBFP, LOG_LEVEL_PARANOID );
    EXPECT_TRUE( tbfp_check_flow_control(IF_LOOPBACK,1,&prev_s_num,&con_flow,&max_flow));
    EXPECT_EQ(1, prev_s_num);
    EXPECT_EQ(1, con_flow);

    EXPECT_TRUE( tbfp_check_flow_control(IF_LOOPBACK,2,&prev_s_num,&con_flow,&max_flow));
    EXPECT_EQ(2, prev_s_num);
    EXPECT_EQ(2, con_flow);

    EXPECT_TRUE( tbfp_check_flow_control(IF_LOOPBACK,5,&prev_s_num,&con_flow,&max_flow));
    EXPECT_EQ(5, prev_s_num);
    EXPECT_EQ(1, con_flow);

    EXPECT_FALSE( tbfp_check_flow_control(IF_LOOPBACK,1,&prev_s_num,&con_flow,&max_flow));
    EXPECT_EQ(1, prev_s_num);
    EXPECT_EQ(1, con_flow);
    return true;
}



