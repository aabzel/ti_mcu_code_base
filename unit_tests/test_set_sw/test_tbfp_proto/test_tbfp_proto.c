#include "test_tbfp_proto.h"

#ifdef X86_64
#include <stdio.h>
#endif

#include "tbfp_protocol.h"
#include "unit_test_check.h"

static const uint8_t tbfp_message[ ] = {};

bool test_tbfp_proto_1(void) {
#ifdef X86_64
    printf("\n%s():", __FUNCTION__);
#endif

    return true;
}

bool test_tbfp_types(void) {
#ifdef X86_64
    printf("\n%s():", __FUNCTION__);
#endif

    return true;
}
