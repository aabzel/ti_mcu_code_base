#include "test_tbfp_rtcm_proto.h"

#ifdef X86_64
#include <stdio.h>
#endif

#include "system.h"
#include "log.h"
#include "test_rtcm3_proto.h"
#include "tbfp_protocol.h"
#include "rtcm3_protocol.h"
#include "unit_test_check.h"

bool test_tbfp_rtcm_path(void) {
#ifdef X86_64
    printf("\n%s():", __FUNCTION__);
#endif
    uint32_t i=0;
    EXPECT_TRUE( set_log_level(TBFP , LOG_LEVEL_DEBUG ));
    EXPECT_TRUE( set_log_level(RTCM, LOG_LEVEL_DEBUG ));
    EXPECT_TRUE(tbfp_protocol_init(&TbfpProtocol[IF_LOOPBACK], IF_LOOPBACK));
    EXPECT_TRUE(rtcm3_protocol_init(&Rtcm3Protocol[IF_LOOPBACK], IF_LOOPBACK, false));
    for (i=1; i<1000;i++) {
        EXPECT_TRUE_NUM(tbfp_send(rtcm3_message, sizeof(rtcm3_message), IF_LOOPBACK, 0),i);
        EXPECT_EQ(i,Rtcm3Protocol[IF_LOOPBACK].rx_pkt_cnt);
    }

    return true;
}

