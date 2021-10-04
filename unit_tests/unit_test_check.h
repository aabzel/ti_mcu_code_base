#ifndef UNIT_TEST_CHECK_H
#define UNIT_TEST_CHECK_H

#ifdef X86_64
#include "unit_test_check_x86.h"
#else
#include "unit_test_check_cortex_m.h"
#endif


#endif /* UNIT_TEST_CHECK_H */
