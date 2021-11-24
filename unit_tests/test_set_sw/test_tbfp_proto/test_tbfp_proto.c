#include "test_tbfp_proto.h"

#ifdef X86_64
#include <stdio.h>
#endif

#include "tbfp_protocol.h"
#include "unit_test_check.h"

static const uint8_t tbfp_message1[ ] = {0xA5,0x00,0x63};
static const uint8_t tbfp_message2[ ] = {0xA5,0x01,0x33,0xa9};

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
